/*
 **
 ** Copyright 2006, The Android Open Source Project
 **
 ** Licensed under the Apache License, Version 2.0 (the "License"); 
 ** you may not use this file except in compliance with the License. 
 ** You may obtain a copy of the License at 
 **
 **    http://www.apache.org/licenses/LICENSE-2.0 
 **
 ** Unless required by applicable law or agreed to in writing, software 
 ** distributed under the License is distributed on an "AS IS" BASIS, 
 ** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
 ** See the License for the specific language governing permissions and 
 ** limitations under the License.
 */
#include"common.h"
#include"opencl.h"
#include <utils/Timers.h>

#define INSIZE (4*1024*1024)
#define OUTSIZE 32768
#define MAX_COUNT 200
static const char kernel_function_buffer[] =
"#undef READ_4 \n"
"#undef READ_16 \n"
"#define INSIZE 8388608 \n"
"#define OUTSIZE 32768  \n"
"#define READ_4(p, x, y, z, w)  x += *p++;   y += *p++; z += *p++; w += *p++;\n"
"#define READ_16(p, x, y, z, w) READ_4(p, x, y, z, w); READ_4(p, x, y, z, w); READ_4(p, x, y, z, w); READ_4(p, x, y, z, w); \n"
"__kernel void memory_read_v1(\n"
"	__global const int* restrict pInput,\n"
"	__global int*       restrict pOutput)\n"
"{\n"
"    int x = 0;\n"
"    int y = 0;\n"
"    int z = 0;\n"
"    int w = 0;\n"
"    int off = get_global_id(0)%OUTSIZE;\n"
"    __global const int* p = pInput+16*16*off;\n"
"    READ_16(p, x, y, z, w);   READ_16(p, x, y, z, w);\n"
"    READ_16(p, x, y, z, w);   READ_16(p, x, y, z, w);\n"
"    READ_16(p, x, y, z, w);   READ_16(p, x, y, z, w);\n"
"    READ_16(p, x, y, z, w);   READ_16(p, x, y, z, w);\n"
"    READ_16(p, x, y, z, w);   READ_16(p, x, y, z, w);\n"
"    READ_16(p, x, y, z, w);   READ_16(p, x, y, z, w);\n"
"    READ_16(p, x, y, z, w);   READ_16(p, x, y, z, w);\n"
"    READ_16(p, x, y, z, w);   READ_16(p, x, y, z, w);\n"
"    y+=x+z+w;\n"
"    pOutput[off] = y;\n"
"}\n"
"__kernel void memory_read_v2(\n"
"	__global const int2* restrict pInput,\n"
"	__global int*        restrict pOutput)\n"
"{\n"
"    int2 x = 0;\n"
"    int2 y = 0;\n"
"    int2 z = 0;\n"
"    int2 w = 0;\n"
"    int off = get_global_id(0)%OUTSIZE;\n"
"    __global const int2* p = pInput+8*16*off;\n"
"    READ_16(p, x, y, z, w);   READ_16(p, x, y, z, w);\n"
"    READ_16(p, x, y, z, w);   READ_16(p, x, y, z, w);\n"
"    READ_16(p, x, y, z, w);   READ_16(p, x, y, z, w);\n"
"    READ_16(p, x, y, z, w);   READ_16(p, x, y, z, w);\n"
"    y+=x+z+w;\n"
"   pOutput[off] = y.lo+y.hi;\n"
"}\n"
"__kernel void memory_read_v4(\n"
"	__global const int4* restrict pInput,\n"
"	__global int*        restrict pOutput)\n"
"{\n"
"    int4 x = 0;\n"
"    int4 y = 0;\n"
"    int4 z = 0;\n"
"    int4 w = 0;\n"
"    int off = get_global_id(0)%OUTSIZE;\n"
"    __global const int4* p = pInput+4*16*off;\n"
"    READ_16(p, x, y, z, w);   READ_16(p, x, y, z, w);\n"
"    READ_16(p, x, y, z, w);   READ_16(p, x, y, z, w);\n"
"    y+=x+z+w;\n"
"    int2 y2 = y.lo+y.hi;\n"
"    pOutput[off] = y2.lo+y2.hi;\n"
"}\n"
"__kernel void memory_read_v8(\n"
"	__global const int8* restrict pInput,\n"
"	__global int*        restrict pOutput)\n"
"{\n"
"    int8 x = 0;\n"
"    int8 y = 0;\n"
"    int8 z = 0;\n"
"    int8 w = 0;\n"
"    int off = get_global_id(0)%OUTSIZE;\n"
"    __global const int8* p = pInput+2*16*off;\n"
"    READ_16(p, x, y, z, w);\n"
"    READ_16(p, x, y, z, w);\n"
"    y+=x+z+w;\n"
"    int4 y4 = y.lo+y.hi;\n"
"    int2 y2 = y4.lo+y4.hi;\n"
"    pOutput[off] = y2.lo+y2.hi;\n"
"}\n"
"__kernel void memory_read_v16(\n"
"	__global const int16* restrict pInput,\n"
"	__global int*         restrict pOutput)\n"
"{\n"
"    int16 x = 0;\n"
"    int16 y = 0;\n"
"    int16 z = 0;\n"
"    int16 w = 0;\n"
"    int off = get_global_id(0)%OUTSIZE;\n"
"    __global const int16* p = pInput+16*off;\n"
"    READ_16(p, x, y, z, w);\n"
"    y+=x+z+w;\n"
"    int8 y8 = y.lo+y.hi;\n"
"    int4 y4 = y8.lo+y8.hi;\n"
"    int2 y2 = y4.lo+y4.hi;\n"
"    pOutput[off] = y2.lo+y2.hi;\n"
"}\n"
"__kernel void memory_write_v1(\n"
"	__global int* restrict pOutput)\n"
"{\n"
"    int x = get_global_id(0)%INSIZE;\n"
"    pOutput[x] = x;\n"
"}\n"
"__kernel void memory_write_v2(\n"
"	__global int2* restrict pOutput)\n"
"{\n"
"    int x = get_global_id(0)%(INSIZE/2);\n"
"    pOutput[x] = (int2)x;\n"
"}\n"
"__kernel void memory_write_v4(\n"
"	__global int4* restrict pOutput)\n"
"{\n"
"    int x = get_global_id(0)%(INSIZE/4);\n"
"    pOutput[x] = (int4)x;\n"
"}\n"
"__kernel void memory_write_v8(\n"
"	__global int8* restrict pOutput)\n"
"{\n"
"    int x = get_global_id(0)%(INSIZE/8);\n"
"    pOutput[x] = (int8)x;\n"
"}\n"
"__kernel void memory_write_v16(\n"
"	__global int16* restrict pOutput)\n"
"{\n"
"    int x = get_global_id(0)%(INSIZE/16);\n"
"    pOutput[x] = (int16)x;\n"
"}\n"
"__kernel void memory_copy_v1(__global const int* pInput,\n"
"	                     __global int* pOutput)\n"
"{\n"
"    int x = get_global_id(0);//%INSIZE; james modify\n"
"    pOutput[x] = pInput[x];\n"
"}\n"
"__kernel void memory_copy_v2(__global const int2* pInput,\n"
"	                     __global int2* pOutput)\n"
"{\n"
"    int x = get_global_id(0)%(INSIZE/2);\n"
"    pOutput[x] = pInput[x];\n"
"}\n"
"__kernel void memory_copy_v4(__global const int4* pInput,\n"
"	                     __global int4* pOutput)\n"
"{\n"
"    int x = get_global_id(0)%(INSIZE/4);\n"
"    pOutput[x] = pInput[x];\n"
"}\n"
"__kernel void memory_copy_v8(__global const int8* pInput,\n"
"	                     __global int8* pOutput)\n"
"{\n"
"    int x = get_global_id(0)%(INSIZE/8);\n"
"    pOutput[x] = pInput[x];\n"
"}\n"
"__kernel void memory_copy_v16(__global const int16* pInput,\n"
"	                      __global int16* pOutput)\n"
"{\n"
"    int x = get_global_id(0)%(INSIZE/16);\n"
"    pOutput[x] = pInput[x];\n"
"}";

static int input[INSIZE];
static int output[INSIZE];
int main(int argc, char **argv)
{
	//OpenCL
	cl_device_id device;
	cl_context context;
	cl_command_queue queue;
	cl_program program;
	cl_kernel kernel;
	size_t global_size,local_size,num;
	cl_int err;
	cl_mem input_buffer,output_buffer;
	char ch;
	int i = 0;

	printf("create_device\n");
	device = create_device();

	printf("clCreateContext\n");
	context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
	if(err < 0) 
	{
		perror("Couldn't create a context");
		exit(1);   
	}

	printf("build_program\n");
	program = build_program(context,device,kernel_function_buffer);

	printf("clCreateCommandQueue\n");
	queue = clCreateCommandQueue(context, device, 0, &err);
	if(err < 0) 
	{
		perror("Couldn't create a command queue");
		exit(1);   
	};

	printf("clCreateKernel\n");
	kernel = clCreateKernel(program,"memory_copy_v1", &err);
	if(err < 0) 
	{
		perror("Couldn't create a kernel");
		exit(1);
	};

	global_size=INSIZE; // xbdong

	printf("clCreateBuffer\n");
	input_buffer = clCreateBuffer(context, CL_MEM_READ_ONLY|CL_MEM_COPY_HOST_PTR, INSIZE*sizeof(int), input, &err);    
	if(err < 0) 
	{
		perror("Couldn't create a buffer");
		exit(1);   
	};
	output_buffer = clCreateBuffer(context, CL_MEM_READ_WRITE, INSIZE*sizeof(int), NULL, &err);
	if(err < 0) 
	{
		perror("Couldn't create a buffer");
		exit(1);   
	};

	printf("clSetKernelArg\n");
	err  = clSetKernelArg(kernel, 0,sizeof(cl_mem), &input_buffer);
	err |= clSetKernelArg(kernel, 1,sizeof(cl_mem), &output_buffer);

	if(err < 0) 
	{
		printf("Couldn't set an argument for the exposure kernel");
		exit(1);   
	};

	printf("press any key to continue\n");
	scanf("%c",&ch);
	nsecs_t now = systemTime();
	for(i = 0; i < MAX_COUNT; i++)
	{
		err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_size, NULL, 0, NULL, NULL);   
		if(err != 0) 
		{
			printf("%d\n",err);
			perror("Couldn't enqueue the exposure kernel\n");
			exit(1);   
		}
		clFinish(queue);
	}
	nsecs_t t = systemTime() - now;
	float second = t/1000000000.0f;
	float band_width = INSIZE/1024.0f/1024.0f/1024.0f*MAX_COUNT*sizeof(int)*2;
	printf("memory %fM -> %fM count=%d:\t time=%fs\tBW=%fG\n", INSIZE*sizeof(int)/1024.0f/1024.0f, INSIZE*sizeof(int)/1024.0f/1024.0f, MAX_COUNT, second, band_width/second);

	clReleaseKernel(kernel);
	clReleaseMemObject(input_buffer);
	clReleaseMemObject(output_buffer);
	clReleaseProgram(program);
	clReleaseCommandQueue(queue);
	clReleaseContext(context);

	printf("end\n");
	return 0;
}



