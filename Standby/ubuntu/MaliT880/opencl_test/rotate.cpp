
//#include <cutils/trace.h>

#include "imagealign.h"
#include "common.h"
#include"opencl.h"
#include"cmr_common.h"

#define DEBUG_DG
#define DEBUG_IMF
//#define COMPARE

#if 1
#include <sys/time.h>
#include <time.h>

static long long gTime;

static long long system_time()
{
	struct timespec t;
	t.tv_sec = t.tv_nsec = 0;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return (long long)(t.tv_sec)*1000000000LL + (long long)t.tv_nsec;
}

static void startTime()
{
	gTime = system_time();
}

static void endTime(const char *str)
{
	long long t = system_time() - gTime;
	double ds = ((double)t) / 1e6;
	printf("Test: %s, %f ms\n", str, ds);

}
#endif


void LoadImg(uchar* yuvData, uchar *output, int w, int h, cl_program program, cl_context context, cl_command_queue queue)
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
	cl_mem img_buffer, out_buffer;

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

	img_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_USE_HOST_PTR, (w*h*3/2)*sizeof(uchar), yuvData, &err);
	if (err < 0) {
		perror("Couldn't create a img buffer");
		exit(1);   
	}
	
	out_buffer = clCreateBuffer(context, CL_MEM_WRITE_ONLY, (w*h*3/2)*sizeof(uchar), NULL, &err);
	if (err < 0)  {
		perror("Couldn't create a out buffer");
		exit(1);   
	}

	startTime();
	// rotateY
	err  = clSetKernelArg(rotate_y_kernel, 0,sizeof(cl_mem), &out_buffer);
	err |= clSetKernelArg(rotate_y_kernel, 1,sizeof(cl_mem), &img_buffer);
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
	err |= clSetKernelArg(rotate_uv_kernel, 1,sizeof(cl_mem), &img_buffer);
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
	err |= clSetKernelArg(rotate_uv_kernel, 1,sizeof(cl_mem), &img_buffer);
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

	err = clEnqueueReadBuffer(queue, out_buffer, CL_TRUE, 0, (w*h*3/2)*sizeof(uchar), output, 0, NULL, NULL);
	if(err < 0) {
		perror("Couldn't read the buffer");
		exit(1);   
	} 
        endTime("calculate YUV");
}

// id0 = width(0, w), id1=height(0, h)
static const char kernel_function_rotate_pY[] =
"__kernel void rotateY(__global uchar *dst, __global uchar *src, int w, int h)	\n"
"{										\n"
"	int g_id_0 = get_global_id(0);						\n"
"	int g_id_1 = get_global_id(1);						\n"
"                                                                               \n"
"	dst[g_id_0*h + (h-1)-g_id_1] = src[w*g_id_1 + g_id_0];                  \n"
"}										\n";

// id0 = halfW(0, halfW), id1 = halfH(1, halfH+1), pBase = wh or wh * 5/4
static const char kernel_function_rotate_pUV[] =
"__kernel void rotateUV(__global uchar *dst, __global uchar *src, int halfW, int halfH, int pBase)	\n"
"{                                                                                                      \n"
"	int g_id_0 = get_global_id(0);                                                                  \n"
"	int g_id_1 = get_global_id(1);                                                                  \n"
"                                                                                                       \n"
"	dst[pBase + (g_id_0 * halfH + (g_id_1-1))] = src[pBase + ((halfH-g_id_1) * halfW) + g_id_0];    \n"
"	//dst[g_id_0*h + (h-1)-g_id_1] = src[w*g_id_1 + g_id_0];                                        \n"
"}													\n";


int GetAlignImg(uchar *imgBuf, int w, int h, uchar *output)
{
	int i;

	int ret=0;
	FILE *fp;

	//OpenCL
	cl_device_id device;
	cl_context context;
	cl_command_queue queue;
	cl_program program;
	cl_int err;

	startTime();
	device = create_device();
	context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
	if (err < 0) {
		perror("Couldn't create a context");
		exit(1);   
	}
	
	//create and build program
	program = build_program(context, device, kernel_function_rotate_pY, kernel_function_rotate_pUV);
	
	//create queue
	queue = clCreateCommandQueue(context, device, 0, &err);
	//if(err < 0) 
	if (err != CL_SUCCESS) {
		perror("Couldn't create a command queue");
		exit(1);   
	}
	endTime("buildKernel");

	LoadImg(imgBuf, output, w, h, program, context, queue);	

	return ret;
}


int SaveImg(char *fileName, uchar *imgData, int w, int h)
{
	FILE  *fp = fopen(fileName, "wb");
	if(NULL == fp)
	{
		printf("Open image file error.\n");
		return -1;
	}
	fwrite(imgData,1,w*h*3/2,fp);//yuv
	fclose(fp);
	return 0;
} 

extern void rotateYUV420SP_normal_pUV(unsigned char *src, unsigned char *des,
        int w, int h);
extern void rotateYUV420SP_normal_pUV_OpenCL(unsigned char *src, unsigned char *des,
        int w, int h);
extern void rotateYUV420SP_normal_pUV_performance1(unsigned char *src, unsigned char *des,
        int w, int h);
extern void rotateYUV420SP_normal_pUV_performance2(unsigned char *src, unsigned char *des,
        int w, int h);

int Rotate_Function(uchar *imgBuf, uchar *output, int h, int w)
{
	int img_cnt=3;
	int ret=0;

	if (0 != GetAlignImg(imgBuf, w, h, output)) {
		printf("GetAlignImg error!");
		ret=-1;
	}

		// 非连续 UV
	startTime();
	rotateYUV420SP_normal_pUV(imgBuf, output, w, h);
	endTime("rotateYUV420SP_normal_pUV");

	startTime();
	rotateYUV420SP_normal_pUV_performance1(imgBuf, output, w, h);
	endTime("rotateYUV420SP_normal_pUV_performance1");

	startTime();
	rotateYUV420SP_normal_pUV_performance2(imgBuf, output, w, h);
	endTime("rotateYUV420SP_normal_pUV_performance2");

	startTime();
	rotateYUV420SP_normal_pUV_OpenCL(imgBuf, output, w, h);
	endTime("rotateYUV420SP_normal_pUV_OpenCL_Hello");

	return ret;
}

