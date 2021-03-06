#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#include "util.h"

void check_error(int error, int line)
{
	if (error != CL_SUCCESS) {
		printf("error is: %d,  line: %d\n", error, line-1);
		exit(EXIT_FAILURE);
	}
}

void get_platform_info(cl_platform_id *platform, int num)
{
	int err;
	size_t len = 100;
	char buf[len];

	printf("[Platform Infomation]\n");
	for (int i = 0; i < num; i++) {
		err = clGetPlatformInfo(platform[i], CL_PLATFORM_NAME, len, buf,
			NULL);
		check_error(err, __LINE__);
		printf("platform name: %s\n", buf);

		printf("\n");
	}
}

void get_devices_info(cl_device_id *devices, int num)
{
	int err;
	size_t len = 100;
	char buf[len];

	printf("[Device Infomation]\n");
	for (int i = 0; i < num; i++) {
		err = clGetDeviceInfo(devices[i], CL_DEVICE_NAME, len, buf, NULL);
		check_error(err, __LINE__);
		printf("device name: %s\n", buf);
	}
}

char *package_program(const char *filename)
{
	FILE *file;
	char *buf;
	long program_size;

	file = fopen(filename, "rb");
	if (!file) {
		perror("open file fail");
		return NULL;
	}

	// 设置文件位置指示符，指向文件末尾
	fseek(file, 0, SEEK_END);

	// 获取文件指示符的当前位置
	program_size = ftell(file);

	// 重置指示符指向文件的起始位置
	rewind(file);

	buf = (char *)malloc(program_size + 1);
	if (!buf) {
		perror("alloc memory fail");
		fclose(file);
		return NULL;
	}
	buf[program_size] = '\0';
	fread(buf, sizeof(char), program_size, file);
	fclose(file);
	return buf;
}

enum OPENCL_DEVS {
	DEVICE_CPU = 0,
	DEVICE_GPU,
	DEVICE_NUM

};

//#define DEVICE_CPU	0
//#define DEVICE_GPU	1

void init_opencl(cl_platform_id *plt, cl_device_id *d, cl_context *c,
	cl_command_queue *q, cl_program *p)
{
	int err;
	cl_platform_id platform;
	cl_device_id device[2];

	cl_context context;
	cl_command_queue queue[2];
	cl_program program;

	char *kernel_source;

	// get platform
	err = clGetPlatformIDs(1, &platform, NULL);
	check_error(err, __LINE__);
	get_platform_info(&platform, 1);

	// cpu & gpu device
	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1,
		&device[DEVICE_CPU], NULL);
	err |= clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1,
		&device[DEVICE_GPU], NULL);
	if (err != CL_SUCCESS) {
		printf("can't get cpu or gpu device\n");
		check_error(err, __LINE__);
	}
	get_devices_info(device, 2);

	// create context
	context = clCreateContext(NULL, 2, device, NULL, NULL, &err);
	if (context == NULL) {
		printf("create context fail\n");
		exit(EXIT_FAILURE);
	}

	// create command queue
	queue[DEVICE_CPU] = clCreateCommandQueue(context, device[DEVICE_CPU],
		CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &err);
	queue[DEVICE_GPU] = clCreateCommandQueue(context, device[DEVICE_GPU],
		CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &err);
	if ((queue[DEVICE_CPU] == NULL) || (queue[DEVICE_GPU] == NULL)) {
		printf("create command queue fail\n");
		exit(EXIT_FAILURE);
	}

	kernel_source = package_program("program.cl");
	if (!kernel_source) {
		printf("alloc program buffer fail\n");
		exit(EXIT_FAILURE);

	}

	// create program
	program = clCreateProgramWithSource(context, 1, (const char **)&kernel_source, NULL, &err);
	if (program == NULL) {
		printf("create program fail\n");
		exit(EXIT_FAILURE);
	}
	free(kernel_source);

	// build program
	err = clBuildProgram(program, 2, device, NULL, NULL, NULL);
	if (CL_SUCCESS != err) {
		size_t bufSize = 1024;
		char buf[bufSize];

		err = clGetProgramBuildInfo(program, device[DEVICE_CPU], CL_PROGRAM_BUILD_LOG,
			bufSize, buf, NULL);
		check_error(err, __LINE__);
		printf("build for cpu log:\n%s\n", buf);

		err = clGetProgramBuildInfo(program, device[DEVICE_GPU], CL_PROGRAM_BUILD_LOG,
			bufSize, buf, NULL);
		check_error(err, __LINE__);
		printf("build for gpu log:\n%s\n", buf);

		exit(EXIT_FAILURE);
	}

	// get return value
	*plt = platform;
	d[DEVICE_CPU] = device[DEVICE_CPU];
	d[DEVICE_GPU] = device[DEVICE_GPU];
	*c = context;
	q[DEVICE_CPU] = queue[DEVICE_CPU];
	q[DEVICE_GPU] = queue[DEVICE_GPU];
	*p = program;
}

int main()
{
	int err;
	cl_platform_id platform;
	cl_device_id device[DEVICE_NUM];

	// should be release
	cl_context context;
	cl_command_queue queue[DEVICE_NUM];
	cl_program program;
	cl_kernel kernel_add, kernel_mul;
	char *program_buf;

	cl_mem mem_obj1, mem_obj2, mem_dst_obj;
	cl_event event_marker, event;
	int *host_data, *dst_buffer;
	size_t size = sizeof(int) * 10 * 1024 * 1024; /* 40MB */

	init_opencl(&platform, device, &context, queue, &program);

	// create memory buffer object
	mem_obj1 = clCreateBuffer(context, CL_MEM_READ_ONLY, size,
		NULL, &err);
	mem_obj2 = clCreateBuffer(context, CL_MEM_READ_ONLY, size,
		NULL, &err);
	if (mem_obj1 == NULL || mem_obj2 == NULL) {
		printf("create memory buffer object fail: %d\n", err);
		exit(EXIT_FAILURE);
	}

	// alloc and initialize host buffer
	host_data = (int *)malloc(size);
	if (!host_data) {
		printf("alloc memory fail\n");
		exit(EXIT_FAILURE);
	} else {
		for (int i = 0; i < (int)(size/sizeof(int)); i++)
			host_data[i] = i;
	}

	// non-block write memory object
	err = clEnqueueWriteBuffer(queue[DEVICE_GPU], mem_obj1, CL_FALSE, 0,
		size, host_data, 0, NULL, NULL);
	check_error(err, __LINE__);

	err = clEnqueueWriteBuffer(queue[DEVICE_GPU], mem_obj2, CL_FALSE, 0,
		size, host_data, 0, NULL, NULL);
	check_error(err, __LINE__);

	clEnqueueMarker(queue[DEVICE_GPU], &event_marker);

	// create destination memory object
	mem_dst_obj= clCreateBuffer(context, CL_MEM_READ_WRITE, size,
		NULL, &err);
	if (mem_dst_obj == NULL) {
		printf("create dst memory buffer object fail: %d\n", err);
		exit(EXIT_FAILURE);
	}

	// create and set kernel argument (add)
	kernel_add = clCreateKernel(program, "kernel_add", &err);
	if (kernel_add == NULL) {
		printf("create kernel fail: %d\n", err);
		exit(EXIT_FAILURE);
	}
	err = clSetKernelArg(kernel_add, 0, sizeof(cl_mem), &mem_dst_obj);
	err |= clSetKernelArg(kernel_add, 1, sizeof(cl_mem), &mem_obj1);
	err |= clSetKernelArg(kernel_add, 2, sizeof(cl_mem), &mem_obj2);
	check_error(err, __LINE__);

	// create and set kernel argument (mul)
	kernel_mul = clCreateKernel(program, "kernel_mul", &err);
	if (kernel_mul == NULL) {
		printf("create kernel fail: %d\n", err);
		exit(EXIT_FAILURE);
	}

	err = clSetKernelArg(kernel_mul, 0, sizeof(cl_mem), &mem_dst_obj);
	err |= clSetKernelArg(kernel_mul, 1, sizeof(cl_mem), &mem_obj1);
	err |= clSetKernelArg(kernel_mul, 2, sizeof(cl_mem), &mem_obj2);
	check_error(err, __LINE__);

	size_t global_size = size / sizeof(int);
	size_t max_work_group_size[DEVICE_NUM];

	clGetDeviceInfo(device[DEVICE_CPU], CL_DEVICE_MAX_WORK_GROUP_SIZE,
		sizeof(size_t), &max_work_group_size[DEVICE_CPU], NULL);
	clGetDeviceInfo(device[DEVICE_GPU], CL_DEVICE_MAX_WORK_GROUP_SIZE,
		sizeof(size_t), &max_work_group_size[DEVICE_GPU], NULL);

	// create destination buffer
	dst_buffer = (int *)malloc(size);
	if (!dst_buffer) {
		printf("alloc memory fail\n");
		exit(EXIT_FAILURE);
	}

	// cpu-queue wait for event marker
	clEnqueueWaitForEvents(queue[DEVICE_CPU], 1, &event_marker);

	// execute kernel. Memory object should be ready
	err = clEnqueueNDRangeKernel(queue[DEVICE_CPU], kernel_add, 1,
		0, &global_size, &max_work_group_size[DEVICE_CPU],
		0, NULL, &event);
	err |= clEnqueueNDRangeKernel(queue[DEVICE_CPU], kernel_mul, 1,
		0, &global_size, &max_work_group_size[DEVICE_CPU],
		1, &event, NULL);
	check_error(err, __LINE__);

	clEnqueueBarrier(queue[DEVICE_CPU]);

	// read destination memory object to buffer
	err = clEnqueueReadBuffer(queue[DEVICE_CPU], mem_dst_obj, CL_TRUE, 0,
		size, dst_buffer, 0, NULL, NULL);
	check_error(err, __LINE__);

	printf("\n[Result]\n");
	// get buffer data
	for (int i = 0; i < (int)(size/sizeof(int)); i++) {
		int data = host_data[i] + host_data[i]; /* dst = src1 + src2 */
		data = data * 10 - host_data[i];/*dst * 10 - src2*/
		if (data != dst_buffer[i]) {
			printf("get result error. index: %d, result:%d\n",
				i, dst_buffer[i]);
			exit(EXIT_FAILURE);
		}
	}

	printf("Success\n");

	clReleaseEvent(event);
	clReleaseEvent(event_marker);
	clReleaseKernel(kernel_add);
	clReleaseKernel(kernel_mul);
	clReleaseMemObject(mem_obj1);
	clReleaseMemObject(mem_obj2);
	clReleaseMemObject(mem_dst_obj);
	clReleaseProgram(program);
	clReleaseCommandQueue(queue[DEVICE_CPU]);
	clReleaseCommandQueue(queue[DEVICE_GPU]);
	clReleaseContext(context);

	return err;
}

// 1.首先在同一个设备上创建两个命令队列；
// 2.接着在 MAC OS 系统上将两个命令队列对应到两个设备上执行；


