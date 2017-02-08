#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>

#define NWITEMS		50
//#define NWITEMS		512
//#define NWITEMS		40960

// A simple memset kernel
const char *source =
"kernel void id_check(global float *output)					\n"
"{                                                                              \n"
"	// get global id, from set                                              \n"
"	size_t global_id_0 = get_global_id(0);                                  \n"
"	size_t global_id_1 = get_global_id(1);                                  \n"
"                                                                               \n"
"	//get global size, from set                                             \n"
"	size_t global_size_0 = get_global_size(0);                              \n"
"										\n"
"	// get global offset, from set                                          \n"
"	size_t offset_0 = get_global_offset(0);                                 \n"
"	size_t offset_1 = get_global_offset(1);                                 \n"
"                                                                               \n"
"	// local id, from set                                                   \n"
"	size_t local_id_0 = get_local_id(0);                                    \n"
"	size_t local_id_1 = get_local_id(1);                                    \n"
"										\n"
"	// get global index, from 0                                             \n"
"	int index_0 = global_id_0 - offset_0;                                   \n"
"	int index_1 = global_id_1 - offset_1;                                   \n"
"                                                                               \n"
"	// important(index):                                                    \n"
"	int index = index_1 * global_size_0 + index_0;                          \n"
"                                                                               \n"
"	float f = global_id_0 * 10.0f + global_id_1 * 1.0f;			\n"
"	//f += local_id_0 * 0.1f + local_id_1 * 0.01f;                            \n"
"                                                                               \n"
"	// order: global_id_0 ## global_id_1 ## local_id_0 ## local_id_1        \n"
"	output[index] = f;                                                      \n"
"} 										\n";


#if 0
const char *source =
"kernel void id_check(global float *output)					\n"
"{                                                                              \n"
"	// get global id, from set                                              \n"
"	size_t global_id_0 = get_global_id(0);                                  \n"
"	size_t global_id_1 = get_global_id(1);                                  \n"
"                                                                               \n"
"	//get global size, from set                                             \n"
"	size_t global_size_0 = get_global_size(0);                              \n"
"										\n"
"	// get global offset, from set                                          \n"
"	size_t offset_0 = get_global_offset(0);                                 \n"
"	size_t offset_1 = get_global_offset(1);                                 \n"
"                                                                               \n"
"	// local id, from set                                                   \n"
"	size_t local_id_0 = get_local_id(0);                                    \n"
"	size_t local_id_1 = get_local_id(1);                                    \n"
"										\n"
"	// get global index, from 0                                             \n"
"	int index_0 = global_id_0 - offset_0;                                   \n"
"	int index_1 = global_id_1 - offset_1;                                   \n"
"                                                                               \n"
"	// important(index):                                                    \n"
"	int index = index_1 * global_size_0 + index_0;                          \n"
"                                                                               \n"
"	float f = global_id_0 * 10.0f + global_id_1 * 1.0f;			\n"
"	f += local_id_0 * 0.1f + local_id_1 * 0.01f;                            \n"
"                                                                               \n"
"	// order: global_id_0 ## global_id_1 ## local_id_0 ## local_id_1        \n"
"	output[index] = f;                                                      \n"
"} 										\n";
#endif

#if 0
const char *source =
"__kernel void memset(__global uint *dst)		\n"
"{							\n"
"	dst[get_global_id(0)] = get_global_id(0);	\n"
"}							\n";
#endif

int main(int argc, char **argv)
{
	// 1.Get platforms
	cl_uint num_platform_ids;
	cl_platform_id *platform_ids;
	clGetPlatformIDs(0, NULL, &num_platform_ids);
	printf("platform ids: %u\n", num_platform_ids);

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

		clGetDeviceIDs(platform_ids[i], CL_DEVICE_TYPE_ALL, 0, NULL, &num_device_ids);
		printf("device ids: %u\n", num_device_ids);

		device_ids = (cl_device_id *)malloc(num_device_ids *
			sizeof(cl_device_id));
		if (device_ids == NULL) {
			printf("malloc error: device ids\n");
			return 0;
		}
		clGetDeviceIDs(platform_ids[i], CL_DEVICE_TYPE_ALL,
			num_device_ids, device_ids, NULL);


		//for (int j = 0; j < num_device_ids; j++) {
		for (int j = 0; j < 1; j++) {
			// 3.Create a context and command queue on that device
			cl_context context = clCreateContext(NULL, 1,
				&device_ids[i], NULL, NULL, NULL);

			cl_command_queue queue = clCreateCommandQueue(context,
				device_ids[i], 0, NULL);


			// 4.Perform runtime source compilation, and obtain
			// kernel entry point.
			cl_program program =
				clCreateProgramWithSource(context, 1, &source,
					NULL, NULL);

			clBuildProgram(program, 1, &device_ids[i], NULL, NULL,
				NULL);

#if 0
			cl_kernel kernel = clCreateKernel(program, "memset",
				NULL);
#endif
			cl_kernel kernel = clCreateKernel(program, "id_check",
				NULL);

			// 5.Create a data buffer
			cl_mem buffer = clCreateBuffer(context,
				//CL_MEM_WRITE_ONLY, NWITEMS * sizeof(cl_uint),
				CL_MEM_WRITE_ONLY, NWITEMS * sizeof(cl_float),
				NULL, NULL);

			// 6.Launch the kernel. Let OpenCL pick the local work
			// size.
			size_t global_work_size = NWITEMS;
			clSetKernelArg(kernel, 0, sizeof(buffer),
				(void *)&buffer);

			//clEnqueueNDRangeKernel(queue, kernel, 1, NULL,
			//	&global_work_size, NULL, 0, NULL, NULL);
			const size_t globalSize[2] = {6, 4};
			const size_t localSize[2] = {3, 2};
			const size_t offset[2] = {3, 2};
			size_t dim = 2;
			clEnqueueNDRangeKernel(queue, kernel, dim, offset,
				globalSize, localSize, 0, NULL, NULL);

			clFinish(queue);


			// 7.Look at the results via synchronous buffer map
			cl_float *ptr;
			ptr = (cl_float *)clEnqueueMapBuffer(queue, buffer,
				CL_TRUE, CL_MAP_READ, 0, NWITEMS *
				sizeof(cl_float), 0, NULL, NULL, NULL);


			//for (int k = 0; k < NWITEMS; k++)
			for (int k = 0; k < 24; k++)
				printf("%d  %f\n", k, ptr[k]);

			printf("============================\n");
			printf("============================\n");

		}
	}


	return 0;
}


