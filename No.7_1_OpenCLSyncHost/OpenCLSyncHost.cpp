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
	
		printf("\n");
	}
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

	cl_mem mem_obj1, mem_obj2;
	int *buffer;
	size_t size = sizeof(int) * 10 * 1024 * 1024; /* 50MB */

	// get platform
	err = clGetPlatformIDs(1, &platform, NULL);
	check_error(err, __LINE__);
	//get_platform_info(&platform, 1);

	// get gpu device
	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	if (err != CL_SUCCESS) {
		printf("can' get cpu device, try cpu...\n");
		err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device, NULL);
		check_error(err, __LINE__);
	}
	//get_devices_info(&device, 1);

	// create context
	context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
	if (context == NULL) {
		printf("create context fail\n");
		exit(EXIT_FAILURE);
	}

	// create command queue
	queue = clCreateCommandQueue(context, device, 0, &err);
	if (queue == NULL) {
		printf("create command queue fail\n");
		exit(EXIT_FAILURE);
	}

	// create memory object
	mem_obj1 = clCreateBuffer(context, CL_MEM_READ_ONLY, size,
		NULL, &err);
	mem_obj2 = clCreateBuffer(context, CL_MEM_READ_ONLY, size,
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
	
	// block write
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

	// non-block write
	time_start();
	err = clEnqueueWriteBuffer(queue, mem_obj1, CL_FALSE, 0,
		size, buffer, 0, NULL, NULL);
	check_error(err, __LINE__);
	time_end("write memory object1 non-block");

	time_start();
	err = clEnqueueWriteBuffer(queue, mem_obj1, CL_FALSE, 0,
		size, buffer, 0, NULL, NULL);
	check_error(err, __LINE__);
	time_end("write memory object2 non-block");

	time_start();
	clFinish(queue);
	time_end("finish command queue");

	free(buffer);
	clReleaseMemObject(mem_obj1);
	clReleaseMemObject(mem_obj2);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);

	return err;
}
