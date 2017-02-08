#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>

#define NWITEMS			64

// A simple add kernel
const char *main_source =
"#include <add.h>						\n"
"#include <mydir/sub.h>						\n"
"__kernel void call_test(global float *a, const float b)	\n"
"{                                                              \n"
"	int index = get_global_id(0);                           \n"
"	a[index] = add(index, b);                               \n"
"	a[index] = sub(a[index]);                               \n"
"}                                                              \n";

// file: add.h
const char *fun_add =
"float add(float a, float b)				\n"
"{                                                      \n"
"	return (a + b) + 100.0;                         \n"
"}                                                      \n";

// file:mydir/sub.h
const char *fun_sub =
"float sub(float val)					\n"
"{                                                      \n"
"	return val - 50.0; 	                        \n"
"}                                                      \n";

int main(int argc, char **argv)
{
	printf("main_source=\n%s\n", main_source);
	printf("fun_add=\n%s\n", fun_add);
	printf("fun_sub=\n%s\n", fun_sub);

	// 1.Get platforms
	cl_uint num_platform_ids;
	cl_platform_id *platform_ids;
	clGetPlatformIDs(0, NULL, &num_platform_ids);
	printf("num of platform ids: %u\n", num_platform_ids);

	platform_ids = (cl_platform_id *)malloc(num_platform_ids * sizeof(cl_platform_id));
	if (platform_ids == NULL) {
		printf("malloc error: platform ids\n");
		return 0;
	}

	clGetPlatformIDs(num_platform_ids, platform_ids, NULL);

	for (int i = 0; i < num_platform_ids; i++) {
		// 2.Find a GPU device
		cl_uint num_device_ids;
		cl_device_id *device_ids;

		//clGetDeviceIDs(platform_ids[i], CL_DEVICE_TYPE_ALL, 0, NULL, &num_device_ids);
		clGetDeviceIDs(platform_ids[i], CL_DEVICE_TYPE_GPU, 0, NULL, &num_device_ids);
		printf("num of device ids: %u\n", num_device_ids);

		device_ids = (cl_device_id *)malloc(num_device_ids *
			sizeof(cl_device_id));
		if (device_ids == NULL) {
			printf("malloc error: device ids\n");
			return 0;
		}
		//clGetDeviceIDs(platform_ids[i], CL_DEVICE_TYPE_ALL,
		clGetDeviceIDs(platform_ids[i], CL_DEVICE_TYPE_GPU,
			num_device_ids, device_ids, NULL);


		for (int j = 0; j < num_device_ids; j++) {
			// 3.Create a context and command queue on that device
			cl_context context = clCreateContext(NULL, 1,
				&device_ids[i], NULL, NULL, NULL);
			if (context == NULL)
				printf("create context error\n");

			cl_command_queue queue = clCreateCommandQueue(context,
				device_ids[i], 0, NULL);
			if (queue == NULL)	
				printf("create context error\n");


			// 4.Perform runtime source compilation, and obtain
			// kernel entry point.
			cl_program prog_cl, prog_head_add, prog_head_sub, prog;
			cl_int err_ret;
			prog_cl = clCreateProgramWithSource(context, 1,
				&main_source, NULL, &err_ret);
			if ((err_ret != CL_SUCCESS) || (prog_cl == NULL))
				printf("create program fail: %d\n", __LINE__);

			prog_head_add = clCreateProgramWithSource(context, 1,
				&fun_add, NULL, &err_ret);
			if ((err_ret != CL_SUCCESS) || (prog_head_add == NULL))
				printf("create program fail: %d\n", __LINE__);

			prog_head_sub = clCreateProgramWithSource(context, 1,
				&fun_sub, NULL, &err_ret);
			if ((err_ret != CL_SUCCESS) || (prog_head_sub == NULL))
				printf("create program fail: %d\n", __LINE__);


			const char *input_head_names[2] = {"add.h",
				"mydir/sub.h"};
			cl_program input_head[2] = {prog_head_add,
				prog_head_sub};

#if 0
			clCompileProgram(prog_cl, 0, NULL, 0, 1, input_head,
				input_head_names, NULL, NULL);
#endif
			//cl_int err = clCompileProgram(prog_cl, 1, &device_ids[i], NULL, 1,
			cl_int err = clCompileProgram(prog_cl, 0, NULL, NULL, 2,
				input_head, input_head_names, NULL, NULL);
			if (err != CL_SUCCESS)
				printf("compile program error: %d\n", err);

			cl_program program = clLinkProgram(context , 1, &device_ids[i],
				NULL, 1, &prog_cl, NULL, NULL, NULL);
			if (program == NULL) {
				printf("link program error\n");
			}

#if 0
			// 4.Perform runtime source compilation, and obtain
			// kernel entry point.
			cl_program program =
				clCreateProgramWithSource(context, 1, &source,
					NULL, NULL);

			clBuildProgram(program, 1, &device_ids[i], NULL, NULL,
				NULL);
#endif

#if 0
			cl_kernel kernel = clCreateKernel(program, "memset",
				NULL);
#endif
			//cl_kernel kernel = clCreateKernel(program, "id_check",
			cl_kernel kernel = clCreateKernel(program, "call_test",
				NULL);
			if (kernel == NULL)
				printf("create kernel error\n");
			
			// 5.Create a data buffer
			cl_mem buffer = clCreateBuffer(context,
				//CL_MEM_WRITE_ONLY, NWITEMS * sizeof(cl_float),
				CL_MEM_READ_WRITE, NWITEMS * sizeof(cl_float),
				NULL, NULL);
			
			// 6.Launch the kernel. Let OpenCL pick the local work
			// size.
			size_t global_work_size = NWITEMS;
			const cl_float lon = 1.0;

			clSetKernelArg(kernel, 0, sizeof(buffer),
				(void *)&buffer);
			clSetKernelArg(kernel, 1, sizeof(cl_float),
				(void *)&lon);

			const size_t offset[1] = {0};
			const size_t globalSize[1] = {NWITEMS};
			const size_t localSize[1] = {64};

			clEnqueueNDRangeKernel(queue, kernel, 1, offset,
				globalSize, localSize, 0, NULL, NULL);
			clFinish(queue);

			// 7.Look at the results via synchronous buffer map
			cl_float *ptr;
			ptr = (cl_float *)clEnqueueMapBuffer(queue, buffer,
				CL_TRUE, CL_MAP_READ, 0, NWITEMS *
				sizeof(cl_float), 0, NULL, NULL, NULL);

			//for (int k = 0; k < NWITEMS; k++)
			for (int k = 0; k < 64; k++) {
				if ((k != 0) && ((k % 6) == 0))
					printf("\n");
				printf("%.2f  ", ptr[k]);
			}

			printf("\n============================\n");
			printf("============================\n");
		}
	}


	return 0;
}


