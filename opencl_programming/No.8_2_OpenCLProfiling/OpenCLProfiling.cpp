#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#define SIZE	(8*1024*1024)	/* 8MB int32 */

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

		size_t resolution;
		err = clGetDeviceInfo(devices[i],
			CL_DEVICE_PROFILING_TIMER_RESOLUTION, sizeof(size_t),
			&resolution, NULL);
		check_error(err, __LINE__);
		printf("profiling timer resolution: %d(ns)\n", (int)resolution);

		printf("\n");
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

int main()
{
	int err;
	cl_uint device_num;
	cl_platform_id platform;
	cl_device_id device;

	// should be release
	cl_context context;
	cl_command_queue queue;
	cl_program program;
	cl_kernel kernel;
	char *program_buf;

	cl_mem input, output;
	cl_event event;
	cl_ulong prof_start, prof_end;

	// get platform
	err = clGetPlatformIDs(1, &platform, NULL);
	check_error(err, __LINE__);
	get_platform_info(&platform, 1);

	// get gpu device
	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	if (err != CL_SUCCESS) {
		printf("can't get gpu device, try cpu...\n");
		err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device, NULL);
		check_error(err, __LINE__);
	}
	get_devices_info(&device, 1);

	// create context
	context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
	if (context == NULL) {
		printf("create context fail\n");
		exit(EXIT_FAILURE);
	}

	// create command queue
	queue = clCreateCommandQueue(context, device,
		CL_QUEUE_PROFILING_ENABLE, &err);
	if (queue == NULL) {
		printf("create command queue fail\n");
		exit(EXIT_FAILURE);
	}

	program_buf = package_program("program.cl");
	if (!program_buf) {
		printf("alloc program buffer fail\n");
		exit(EXIT_FAILURE);

	}

	// create program
	program = clCreateProgramWithSource(context, 1, (const char **)&program_buf, NULL, &err);
	if (program == NULL) {
		printf("create program fail\n");
		exit(EXIT_FAILURE);
	}
	free(program_buf);

	err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
	if (CL_SUCCESS != err) {
		size_t bufSize = 1024;
		char buf[bufSize];

		err = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG,
			bufSize, buf, NULL);
		check_error(err, __LINE__);
		printf("build log:\n%s\n", buf);
		exit(EXIT_FAILURE);
	}

	// alloc memory
	int *orig_buffer = (int *)malloc(sizeof(int) * SIZE);
	int *dest_buffer = (int *)malloc(sizeof(int) * SIZE);
	if (!orig_buffer || !dest_buffer) {
		printf("alloc memory buffer fail\n");
		exit(EXIT_FAILURE);
	}

	// create memory object
	input = clCreateBuffer(context, CL_MEM_READ_ONLY |
		CL_MEM_COPY_HOST_PTR, sizeof(int)*SIZE, orig_buffer, &err);
	output = clCreateBuffer(context, CL_MEM_WRITE_ONLY, sizeof(int)*SIZE,
		NULL, &err);
	if (input == NULL || output == NULL) {
		printf("create memory buffer fail: %d\n", err);
		exit(EXIT_FAILURE);
	}

	// create kernel
	kernel = clCreateKernel(program, "memory_copy_v1", &err);
	if (kernel == NULL) {
		printf("create kernel fail: %d\n", err);
		exit(EXIT_FAILURE);
	}

	// set kernel argument
	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &output);
	err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &input);
	check_error(err, __LINE__);

	// execute kernel
	size_t g_size[] = {SIZE};
	size_t local_size[] = {256};

	err = clEnqueueNDRangeKernel(queue, kernel, 1,
		NULL, g_size, local_size,
		0, NULL, &event);
	clFinish(queue);

	// 当使用 event 标识的命令执行时，获取以纳秒为单位的时间戳信息
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_START,
		sizeof(cl_ulong), &prof_start, NULL);
	// 获取命令已经执行完成后的时间戳数据
	clGetEventProfilingInfo(event, CL_PROFILING_COMMAND_END,
		sizeof(cl_ulong), &prof_end, NULL);

	printf("[Result]\n");
	printf("profiling kernel time: %.2f us\n", (prof_end-prof_start)/1e3);

	clReleaseEvent(event);
	clReleaseKernel(kernel);
	clReleaseMemObject(input);
	clReleaseMemObject(output);
	clReleaseProgram(program);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);

	return err;
}
