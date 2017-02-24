#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif


/**
 * 计算程序执行时间
 */
static int64_t g_time;

static int64_t system_time()
{
	struct timespec t;
	t.tv_sec = t.tv_nsec = 0;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return (int64_t)(t.tv_sec)*1000000000LL + (int64_t)t.tv_nsec;
}

static void time_start()
{
	g_time = system_time();
}

static void time_end(const char *str)
{
	int64_t t = system_time() - g_time;
	double ds = ((double)t) / 1e6;
	printf("Test: %s, %f ms\n", str, ds);

}

/**
 * 获取代码错误位置
 */
void check_error(int error, int line)
{
	if (error != CL_SUCCESS) {
		printf("error is: %d,  line: %d\n", error, line-1);
		exit(EXIT_FAILURE);
	}
}

/**
 * 将文件中的内容保存到缓冲区中
 */
char *package_program(const char *filename)
{
	FILE *file;
	char *buf;
	long program_size;

	file = fopen(filename, "rb");
	if (!file) {
		perror("open file fail when package program");
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

/**
 * 获取 GPU 信息并初始化
 */
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

/**
 * 读取 kernel 代码，存放到缓冲区中
 */
//void init_opencl(cl_context *context, cl_command_queue *queue, cl_program *program)
void init_opencl(cl_context *c, cl_command_queue *q, cl_program *p)
{
	int err;
	cl_platform_id platform;
	cl_device_id device;

	cl_context context;
	cl_command_queue queue;
	cl_program program;

	char *rotate;

	// get platform
	err = clGetPlatformIDs(1, &platform, NULL);
	check_error(err, __LINE__);
	//get_platform_info(&platform, 1);

	// get gpu device
	err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	if (err != CL_SUCCESS) {
		printf("can' get cpu device, try cpu...\n");
		err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_CPU,
			1, &device, NULL);
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

	rotate = package_program("kernel_rotate.cl");
	if (!rotate) {
		printf("alloc program buffer fail\n");
		exit(EXIT_FAILURE);

	}

	// create program
	program = clCreateProgramWithSource(context, 1, (const char **)&rotate, NULL, &err);
	if (program == NULL) {
		printf("create program fail\n");
		exit(EXIT_FAILURE);
	}
	free(rotate);

	// build program
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

	// get return value
	*c = context;
	*q = queue;
	*p = program;
}


/**
 * 使用 OpenCL 旋转图像。将原缓冲区中的图像顺时针旋
 * 转 90 度后存入目标缓冲区。
 */
void rotate(uint8_t *src, uint8_t *des, int w, int h)
{
	int i,j,n;

	// should be release
	cl_context context;
	cl_command_queue queue;
	cl_program program;
	cl_kernel rotate_kernel;

	init_opencl(&context, &queue, &program);

	size_t global_y_size[2];
	global_y_size[0] = w;
	global_y_size[1] = h;

	size_t local_y_size[2] = {16, 16};

	cl_int err;
	cl_mem in_buffer, out_buffer;

	rotate_kernel = clCreateKernel(program, "rotate_y", &err);
	if (err != CL_SUCCESS) {
		printf("Couldn't create rotateY kernel(%d)\n", err);
		exit(EXIT_FAILURE);
	}

	in_buffer = clCreateBuffer(context,
		CL_MEM_READ_ONLY|CL_MEM_USE_HOST_PTR, (w*h*3/2)*sizeof(uint8_t), src, &err);
	if (err < 0) {
		perror("Couldn't create a img buffer");
		exit(EXIT_FAILURE);   
	}

	out_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, (w*h*3/2)*sizeof(uint8_t), NULL, &err);
	if (err < 0)  {
		perror("Couldn't create a out buffer");
		exit(EXIT_FAILURE);   
	}

	time_start();
	// rotate
	err  = clSetKernelArg(rotate_kernel, 0,sizeof(cl_mem), &in_buffer);
	err |= clSetKernelArg(rotate_kernel, 1,sizeof(cl_mem), &out_buffer);
	err |= clSetKernelArg(rotate_kernel, 2,sizeof(int), &w);
	err |= clSetKernelArg(rotate_kernel, 3,sizeof(int), &h);
	if (err != CL_SUCCESS) {
		printf("Couldn't set an argument for the exposure rotateY kernel");
		exit(EXIT_FAILURE);   
	}

	//printf("global_y_size=%d, %d  local_y_size=%d, %d\n",
	//	(int)global_y_size[0], (int)global_y_size[1],
	//	(int)local_y_size[0], (int)local_y_size[1]);
	err = clEnqueueNDRangeKernel(queue, rotate_kernel, 2, NULL, global_y_size, local_y_size, 0, NULL, NULL);   
	if (err != CL_SUCCESS) {
		printf("Couldn't enqueue the exposure rotateY kernel(%d)\n", err);
		exit(EXIT_FAILURE);   
	}

	err = clEnqueueReadBuffer(queue, out_buffer, CL_TRUE, 0,
		(w*h*3/2)*sizeof(uint8_t), des, 0, NULL, NULL);
	if(err < 0) {
		perror("Couldn't read the buffer");
		exit(EXIT_FAILURE);   
	} 
	time_end("yuv420p_rotate_opencl");

	clReleaseKernel(rotate_kernel);
	clReleaseMemObject(in_buffer);
	clReleaseMemObject(out_buffer);
	clReleaseProgram(program);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);
}
