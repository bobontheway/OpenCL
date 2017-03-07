#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#include "util.h"

const char *source =
"__kernel void tolower(__global char *in, __global char *out)		\n"
"{								        \n"
"	int g_id = get_global_id(0);				        \n"
"                                                                       \n"
"	if ((in[g_id] >= 'A') && (in[g_id] <= 'Z'))               	\n"
"		out[g_id] = in[g_id] + 32                        	\n"
"	else                                                            \n"
"		out[g_id] = in[g_id];                             	\n"
"}								        \n";

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
	
		printf("\n");
	}
}

int main()
{
	int err;
	cl_uint device_num;
	cl_platform_id platform;
	cl_device_id *devices;
	cl_device_id device;

	// should be release
	cl_context context;
	cl_command_queue queue;
	cl_program program;
	cl_kernel kernel;

	cl_mem mem_obj1, mem_obj2;
	int *buffer;
	size_t size = sizeof(int) * 10 * 1024 * 1024; /* 50MB */
	const char *upper_case = "Hello OpenCL, I like U";

	// get platform
	err = clGetPlatformIDs(1, &platform, NULL);
	check_error(err, __LINE__);
	get_platform_info(&platform, 1);

	// get gpu device
	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	if (err != CL_SUCCESS) {
		printf("can' get cpu device, try cpu...\n");
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
	//queue = clCreateCommandQueue(context, device, 0, &err);
	// Q: third arg is 0 or NULL
	queue = clCreateCommandQueue(context, device, 0, &err);
	if (queue == NULL) {
		printf("create command queue fail\n");
		exit(EXIT_FAILURE);
	}

#if 0
	// create program
	program = clCreateProgramWithSource(context, 1, &source, NULL, &err);
	if (program == NULL) {
		printf("create program fail\n");
		exit(EXIT_FAILURE);
	}

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
#endif

	// create memory object
	mem_obj1 = clCreateBuffer(context, CL_MEM_HOST_WRITE_ONLY, size,
		NULL, &err);
	mem_obj2 = clCreateBuffer(context, CL_MEM_HOST_WRITE_ONLY, size,
		NULL, &err);
	if (mem_obj1 == NULL || mem_obj2 == NULL) {
		printf("create memory buffer fail: %d\n", err);
		exit(EXIT_FAILURE);
	}

	buffer = (int *)malloc(size);
	if (!buffer) {
		printf("alloc memory fail\n");
		exit(EXIT_FAILURE);
	}
	
	time_start();
	err = clEnqueueWriteBuffer(queue, mem_obj1, CL_TRUE, 0,
		size, buffer, 0, NULL, NULL);
	check_error(err, __LINE__);
	time_end("write memory object1");

	time_start();
	err = clEnqueueWriteBuffer(queue, mem_obj1, CL_TRUE, 0,
		size, buffer, 0, NULL, NULL);
	check_error(err, __LINE__);
	time_end("write memory object2");
#if 0
	// create kernel
	kernel = clCreateKernel(program, "tolower", &err);
	if (kernel == NULL) {
		printf("create kernel fail: %d\n", err);
		exit(EXIT_FAILURE);
	}

	// set kernel argument
	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &mem_obj1);
	err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &mem_obj2);
	check_error(err, __LINE__);

	// execute kernel
	// Q: how to set local size?
	// A: the size same as the strlen
	size_t g_size[] = {strlen(upper_case)};
	size_t local_size[] = {strlen(upper_case)};
	err = clEnqueueNDRangeKernel(queue, kernel, 1,
		0, g_size, local_size,
		0, NULL, NULL);
#endif
	clFinish(queue);

#if 0
	char outBuf[strlen(upper_case)+1];
	outBuf[strlen(upper_case)] = '\0';
#endif
#if 0
	err = clEnqueueReadBuffer(queue, mem_obj2, CL_TRUE, 0,
		strlen(upper_case), outBuf, 0, NULL, NULL);
	check_error(err, __LINE__);
#endif
#if 0
	printf("[Result]\n");
	printf("lower case is: %s\n", outBuf);
#endif
#if 0
	clReleaseKernel(kernel);
#endif
	clReleaseMemObject(mem_obj1);
	clReleaseMemObject(mem_obj2);
#if 0
	clReleaseProgram(program);
#endif
	clReleaseCommandQueue(queue);
	clReleaseContext(context);

	return err;
}
