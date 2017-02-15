#include <sys/time.h>
#include <time.h>

static int64_t g_time;

static int64_t system_time()
{
	struct timespec t;
	t.tv_sec = t.tv_nsec = 0;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return (int64_t)(t.tv_sec)*1000000000LL + (int64_t)t.tv_nsec;
}

//static void time_start()
static void time_start()
{
	g_time = system_time();
}

//static void time_end(const char *str)
static void time_end(const char *str)
{
	int64_t t = system_time() - g_time;
	double ds = ((double)t) / 1e6;
	printf("Test: %s, %f ms\n", str, ds);

}

/**
 * 将文件中的数据加载到缓冲区中
 */
static void load_data(const char *file, uint8_t *addr, uint32_t w, uint32_t h)
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
static void store_data(const char *file, void *addr, uint32_t w, uint32_t h)
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
 * 使用 OpenCL 在 GPU 上执行旋转操作
 */
void yuv420p_rotate_opencl(uint8_t *src, uint8_t *des, int w, int h,
	cl_program program, cl_context context, cl_command_queue queue)
{
	//w = 16;
	//h = 16;
	//w = 32;
	//h = 32;
#if 0
	w = 256;
	h = 256;

	w = 512;
	h = 512;

	w = 1024;
	h = 1024;
#endif

	int i,j,n;
	int halfW = w / 2;
	int halfH = h / 2;
	int baseU = w * h;
	int baseV = w * h * 5 / 4;

	//opencl
	cl_kernel rotate_y_kernel, rotate_uv_kernel;

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

	rotate_y_kernel = clCreateKernel(program, "rotateY", &err);
	if (err != CL_SUCCESS) {
		printf("Couldn't create rotateY kernel(%d)", err);
		//perror("Couldn't create rotateY kernel");
		exit(1);
	}

	rotate_uv_kernel = clCreateKernel(program, "rotateUV", &err);
	if (err != CL_SUCCESS) {
		printf("Couldn't create rotateUV kernel(%d)\n", err);
		//perror("Couldn't create rotateUV kernel");
		exit(1);
	}

	in_buffer = clCreateBuffer(context,
		CL_MEM_READ_ONLY|CL_MEM_USE_HOST_PTR, (w*h*3/2)*sizeof(uint8_t), src, &err);
	if (err < 0) {
		perror("Couldn't create a img buffer");
		exit(1);   
	}

	out_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, (w*h*3/2)*sizeof(uint8_t), NULL, &err);
	if (err < 0)  {
		perror("Couldn't create a out buffer");
		exit(1);   
	}

	time_start();
	// rotateY
	err  = clSetKernelArg(rotate_y_kernel, 0,sizeof(cl_mem), &out_buffer);
	err |= clSetKernelArg(rotate_y_kernel, 1,sizeof(cl_mem), &in_buffer);
	err |= clSetKernelArg(rotate_y_kernel, 2,sizeof(int), &w);
	err |= clSetKernelArg(rotate_y_kernel, 3,sizeof(int), &h);
	if (err != CL_SUCCESS) {
		printf("Couldn't set an argument for the exposure rotateY kernel");
		exit(1);   
	}

	printf("global_y_size=%d, %d  local_y_size=%d, %d\n",
		global_y_size[0], global_y_size[1],
		local_y_size[0], local_y_size[1]);
	err = clEnqueueNDRangeKernel(queue, rotate_y_kernel, 2, NULL, global_y_size, local_y_size, 0, NULL, NULL);   
	if (err != CL_SUCCESS) {
		printf("Couldn't enqueue the exposure rotateY kernel(%d)\n", err);
		//perror("Couldn't enqueue the exposure rotateY kernel\n");
		exit(1);   
	}

	// rotateU
	err  = clSetKernelArg(rotate_uv_kernel, 0,sizeof(cl_mem), &out_buffer);
	err |= clSetKernelArg(rotate_uv_kernel, 1,sizeof(cl_mem), &in_buffer);
	err |= clSetKernelArg(rotate_uv_kernel, 2,sizeof(int), &halfW);
	err |= clSetKernelArg(rotate_uv_kernel, 3,sizeof(int), &halfH);
	err |= clSetKernelArg(rotate_uv_kernel, 4,sizeof(int), &baseU);
	if(err < 0) {
		printf("Couldn't set an argument for the exposure rotateU kernel");
		exit(1);   
	}

	err = clEnqueueNDRangeKernel(queue, rotate_uv_kernel, 2, global_uv_offset, global_uv_size, local_uv_size, 0, NULL, NULL);   
	if(err != 0) 
	{
		printf("%d\n",err);
		perror("Couldn't enqueue the exposure rotateU kernel\n");
		exit(1);   
	}

	// rotateV
	err  = clSetKernelArg(rotate_uv_kernel, 0,sizeof(cl_mem), &out_buffer);
	err |= clSetKernelArg(rotate_uv_kernel, 1,sizeof(cl_mem), &in_buffer);
	err |= clSetKernelArg(rotate_uv_kernel, 2,sizeof(int), &halfW);
	err |= clSetKernelArg(rotate_uv_kernel, 3,sizeof(int), &halfH);
	err |= clSetKernelArg(rotate_uv_kernel, 4,sizeof(int), &baseV);
	if (err < 0) {
		printf("Couldn't set an argument for the exposure rotateV kernel");
		exit(1);   
	}

	err = clEnqueueNDRangeKernel(queue, rotate_uv_kernel, 2, global_uv_offset, global_uv_size, local_uv_size, 0, NULL, NULL);   
	if(err != 0) {
		printf("%d\n",err);
		perror("Couldn't enqueue the exposure rotateV kernel\n");
		exit(1);   
	}

	err = clEnqueueReadBuffer(queue, out_buffer, CL_TRUE, 0,
		(w*h*3/2)*sizeof(uint8_t), des, 0, NULL, NULL);
	if(err < 0) {
		perror("Couldn't read the buffer");
		exit(1);   
	} 
	time_end("calculate YUV");
}

/**
 * 旋转图像。将原缓冲区中的图像顺时针旋转 90 度后存入目标缓冲区
 */
int rotate(uint8_t *img_src, uint8_t *img_dst, int w, int h)
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

	return ret;
}


