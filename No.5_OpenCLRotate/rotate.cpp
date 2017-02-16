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
 * 将文件中的数据加载到缓冲区中
 */
void load_data(const char *file, uint8_t *addr, uint32_t w, uint32_t h)
{
	FILE *fp = NULL;
	uint32_t size = w * h * 3/2;

	fp = fopen(file, "rb");
	if (NULL != fp) {
		fread(addr, 1, size, fp);
		fclose(fp);
	} else {
		perror("open file fail when load data");
	}
}

/**
 * 把缓冲区中的数据存入文件
 */
void store_data(const char *file, void *addr, uint32_t w, uint32_t h)
{
	FILE *fp = NULL;
	uint32_t size = w * h * 3/2;

	fp = fopen(file, "wb");
	if(0 != fp) {
		fwrite((void*)addr, 1, size, fp);
		fclose(fp);
	} else {
		perror("open file fail when store data");
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

/**
 * 使用 C 语言在 CPU 上执行旋转操作。
 */
void yuv420p_rotate_normal(uint8_t *src, uint8_t *des, int w, int h)
{
	int i, j;
	int wh = w * h;

	// 旋转 Y 分量，按照列对原始图像取值
	int k = 0;  
	for(i = 0; i < w; i++) {
		for(j = h-1; j >= 0; j--) {  
			des[k] = src[w*j + i];
			k++;  
		}  
	}

	// 旋转 U 分量
	for (i = 0; i < w/2; i++) {
		for (j = 1; j <= h/2; j++) {
			des[k] = src[wh + ((h/2 - j) * (w/2) + i)];
			k++;
		}
	}

	// 旋转 Ｖ 分量
	for (i = 0; i < w/2; i++) {
		for (j = 1; j <= h/2; j++) {
			des[k] = src[wh+wh/4 + ((h/2 - j) * (w/2) + i)];
			k++;
		}
	}
}

/**
 * 使用 C 语言在 CPU 上执行旋转操作。将除法替换为移位操作
 */
void yuv420p_rotate_shift(uint8_t *src, uint8_t *des, int w, int h)
{
	int i, j;
	int wh = w * h;

	// 旋转 Y 分量，按照列对原始图像取值
	int k = 0;  
	for(i = 0; i < w; i++) {
		for(j = h-1; j >= 0; j--) {  
			des[k] = src[w*j + i];
			k++;  
		}  
	}

	// 旋转 U 分量
	for (i = 0; i < (w>>1); i++) {
		for (j = 1; j <= (h>>1); j++) {
			des[k] = src[wh + (((h>>1) - j) * (w>>1) + i)];
			k++;
		}
	}

	// 旋转 Ｖ 分量
	for (i = 0; i < (w>>1); i++) {
		for (j = 1; j <= (h>>1); j++) {
			des[k] = src[wh+(wh>>2) + (((h>>1) - j) * (w>>1) + i)];
			k++;
		}
	}
}

/**
 * 使用 C 语言在 CPU 上执行旋转操作。把循环中的移位操作去掉
 */
void yuv420p_rotate_delete_shift(uint8_t *src, uint8_t *des, int w, int h)
{
	int i, j;
	int wh = w * h;

	// 旋转 Y 分量，按照列对原始图像取值
	int k = 0;  
	for(i = 0; i < w; i++) {
		for(j = h-1; j >= 0; j--) {  
			des[k] = src[w*j + i];
			k++;  
		}  
	}

	int halfH = h >> 1,
	    halfW = w >> 1,
	    halfUV = wh >> 2;

	// 旋转 U 分量
	for (i = 0; i < halfW; i++) {
		for (j = 1; j <= halfH; j++) {
			des[k] = src[wh + ((halfH - j) * halfW + i)];
			k++;
		}
	}

	// 旋转 Ｖ 分量
	for (i = 0; i < halfW; i++) {
		for (j = 1; j <= halfH; j++) {
			des[k] = src[wh+(wh>>2) + ((halfH - j) * halfW + i)];
			k++;
		}
	}
}

/**
 * 该算法用于 OpenCL 在 GPU 上执行旋转操作
 */
void yuv420p_rotate_opencl_use(uint8_t *src, uint8_t *des, int w, int h)
{
	int i, j;
	int wh = w * h;

	// 旋转 Y 分量，按照列对原始图像取值
	for(i = 0; i < w; i++)
		for(j = h-1; j >= 0; j--)
			des[i * h + (h-1)-j] = src[w*j + i];

	int halfH = h >> 1,
	    halfW = w >> 1,
	    halfUV = wh >> 2;

	// 旋转 U 分量
	for (i = 0; i < halfW; i++)
		for (j = 1; j <= halfH; j++)
			des[wh + (i * halfH + (j-1))] =
				src[wh + ((halfH - j) * halfW + i)];

	// 旋转 Ｖ 分量
	for (i = 0; i < halfW; i++)
		for (j = 1; j <= halfH; j++)
			des[(wh+wh/4) + (i * halfH + (j-1))] =
				src[wh+halfUV + ((halfH - j) * halfW + i)];
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
void init_opencl(cl_context *context, cl_command_queue *queue, cl_program *program)
{
	int err;
	cl_platform_id platform;
	cl_device_id device;

	char *rotate_y, *rotate_uv;
	char *program_buffer[2];

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
	queue = clCreateCommandQueue(context, device, 0, &err);
	if (queue == NULL) {
		printf("create command queue fail\n");
		exit(EXIT_FAILURE);
	}

	rotate_y = package_program("rotate_y.cl");
	if (!rotate_y) {
		printf("alloc program rotate-y buffer fail\n");
		exit(EXIT_FAILURE);

	}
	
	rotate_uv = package_program("rotate_uv.cl");
	if (!rotate_uv) {
		printf("alloc program rotate-uv buffer fail\n");
		exit(EXIT_FAILURE);

	}

	program_buffer[2] = {rotate_y, rotate_uv};

	// create program
	program = clCreateProgramWithSource(context, 1, (const char **)program_buffer, NULL, &err);
	if (program == NULL) {
		printf("create program fail\n");
		exit(EXIT_FAILURE);
	}
	free(rotate_y);
	free(rotate_uv);

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
}

/**
 * 使用 OpenCL 在 GPU 上执行旋转操作
 */
void yuv420p_rotate_opencl(uint8_t *src, uint8_t *des, int w, int h)
{
	int i,j,n;
	int halfW = w / 2;
	int halfH = h / 2;
	int baseU = w * h;
	int baseV = w * h * 5 / 4;

	// should be release
	cl_context context;
	cl_command_queue queue;
	cl_program program;
	cl_kernel rotate_y_kernel, rotate_uv_kernel;

	init_opencl(&context, &queue, &program)

	size_t global_y_size[2];
	global_y_size[0] = w;
	global_y_size[1] = h;

	//size_t local_y_size[2] = {4, 4};
	size_t local_y_size[2] = {16, 16};
	//size_t local_y_size[2] = {48, 48}; error

	size_t global_uv_size[2];
	global_uv_size[0] = w/2;
	global_uv_size[1] = h/2;

	//size_t local_uv_size[2] = {32, 24};
	size_t local_uv_size[2] = {8, 8};

	size_t global_uv_offset[2] = {0, 1};

	cl_int err;
	cl_mem in_buffer, out_buffer;

	rotate_y_kernel = clCreateKernel(program, "rotate_y", &err);
	if (err != CL_SUCCESS) {
		printf("Couldn't create rotateY kernel(%d)", err);		
		exit(EXIT_FAILURE);
	}

	rotate_uv_kernel = clCreateKernel(program, "rotate_uv", &err);
	if (err != CL_SUCCESS) {
		printf("Couldn't create rotateUV kernel(%d)\n", err);
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
	// rotateY
	err  = clSetKernelArg(rotate_y_kernel, 0,sizeof(cl_mem), &in_buffer);
	err |= clSetKernelArg(rotate_y_kernel, 1,sizeof(cl_mem), &out_buffer);
	err |= clSetKernelArg(rotate_y_kernel, 2,sizeof(int), &w);
	err |= clSetKernelArg(rotate_y_kernel, 3,sizeof(int), &h);
	if (err != CL_SUCCESS) {
		printf("Couldn't set an argument for the exposure rotateY kernel");
		exit(EXIT_FAILURE);   
	}

	printf("global_y_size=%d, %d  local_y_size=%d, %d\n",
		global_y_size[0], global_y_size[1],
		local_y_size[0], local_y_size[1]);
	err = clEnqueueNDRangeKernel(queue, rotate_y_kernel, 2, NULL, global_y_size, local_y_size, 0, NULL, NULL);   
	if (err != CL_SUCCESS) {
		printf("Couldn't enqueue the exposure rotateY kernel(%d)\n", err);
		//perror("Couldn't enqueue the exposure rotateY kernel\n");
		exit(EXIT_FAILURE);   
	}

	// rotateU
	err  = clSetKernelArg(rotate_uv_kernel, 0,sizeof(cl_mem), &in_buffer);
	err |= clSetKernelArg(rotate_uv_kernel, 1,sizeof(cl_mem), &out_buffer);
	err |= clSetKernelArg(rotate_uv_kernel, 2,sizeof(int), &halfW);
	err |= clSetKernelArg(rotate_uv_kernel, 3,sizeof(int), &halfH);
	err |= clSetKernelArg(rotate_uv_kernel, 4,sizeof(int), &baseU);
	if(err < 0) {
		printf("Couldn't set an argument for the exposure rotateU kernel");
		exit(EXIT_FAILURE);   
	}

	err = clEnqueueNDRangeKernel(queue, rotate_uv_kernel, 2, global_uv_offset, global_uv_size, local_uv_size, 0, NULL, NULL);   
	if(err != 0) 
	{
		printf("%d\n",err);
		perror("Couldn't enqueue the exposure rotateU kernel\n");
		exit(EXIT_FAILURE);   
	}

	// rotateV
	err  = clSetKernelArg(rotate_uv_kernel, 0,sizeof(cl_mem), &in_buffer);
	err |= clSetKernelArg(rotate_uv_kernel, 1,sizeof(cl_mem), &out_buffer);
	err |= clSetKernelArg(rotate_uv_kernel, 2,sizeof(int), &halfW);
	err |= clSetKernelArg(rotate_uv_kernel, 3,sizeof(int), &halfH);
	err |= clSetKernelArg(rotate_uv_kernel, 4,sizeof(int), &baseV);
	if (err < 0) {
		printf("Couldn't set an argument for the exposure rotateV kernel");
		exit(EXIT_FAILURE);   
	}

	err = clEnqueueNDRangeKernel(queue, rotate_uv_kernel, 2, global_uv_offset, global_uv_size, local_uv_size, 0, NULL, NULL);   
	if(err != 0) {
		printf("%d\n",err);
		perror("Couldn't enqueue the exposure rotateV kernel\n");
		exit(EXIT_FAILURE);   
	}

	err = clEnqueueReadBuffer(queue, out_buffer, CL_TRUE, 0,
		(w*h*3/2)*sizeof(uint8_t), des, 0, NULL, NULL);
	if(err < 0) {
		perror("Couldn't read the buffer");
		exit(EXIT_FAILURE);   
	} 
	time_end("calculate YUV");	

	clReleaseKernel(rotate_y_kernel);
	clReleaseKernel(rotate_uv_kernel);
	clReleaseMemObject(in_buffer);
	clReleaseMemObject(out_buffer);
	clReleaseProgram(program);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);
}

/**
 * 旋转图像。将原缓冲区中的图像顺时针旋转 90 度后存入目标缓冲区
 */
void rotate(uint8_t *img_src, uint8_t *img_dst, int w, int h)
{
	yuv420p_rotate_opencl(img_src, w, h, img_dst);

	time_start();
	yuv420p_rotate_normal(img_src, img_dst, w, h);
	time_end("yuv420p_rotate_normal");

	time_start();
	yuv420p_rotate_shift(img_src, img_dst, w, h);
	time_end("yuv420p_rotate_shift");

	time_start();
	yuv420p_rotate_delete_shift(img_src, img_dst, w, h);
	time_end("yuv420p_rotate_delete_shift");

	time_start();
	yuv420p_rotate_opencl_use(img_src, img_dst, w, h);
	time_end("yuv420p_rotate_opencl_use");
}

