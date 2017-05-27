#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>

/* Find a GPU or CPU associated with the first available platform */
cl_device_id create_device() {

	cl_platform_id platform;
	cl_device_id device;
	int err;
	char* ext_data;							
	size_t ext_size;  
	cl_platform_id *platforms;
	cl_uint num_platforms;
	int i;

	err = clGetPlatformIDs(1, NULL, &num_platforms);		
	if(err < 0) {		
		perror("Couldn't find any platforms.");			
		exit(1);							
	}									

	/* Access all installed platforms */
	platforms = (cl_platform_id*) 					
		malloc(sizeof(cl_platform_id) * num_platforms);		
	clGetPlatformIDs(num_platforms, platforms, NULL);	

	for(i=0; i<(int)num_platforms; i++) {
		err = clGetPlatformInfo(platforms[i], 			
			CL_PLATFORM_NAME, 0, NULL, &ext_size);		
		if(err < 0) {
			perror("Couldn't read extension data.");			
			exit(1);
		}	

		ext_data = (char*)malloc(ext_size);				
		clGetPlatformInfo(platforms[i], CL_PLATFORM_NAME, 	
			ext_size, ext_data, NULL);	
		// printf("Platform supports extensions: %s\n", ext_data);
	}
	//end


	/* Access a device */
	err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	if(err == CL_DEVICE_NOT_FOUND) {
		err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_CPU, 1, &device, NULL);
	}
	if(err < 0) {
		perror("Couldn't access any devices");
		exit(1);   
	}



	return device;
}


/* Create program from a file and compile it */
cl_program build_program(cl_context context, cl_device_id device, const char* program_buffer ) 
{
	cl_program program;
	char *program_log;
	//bool isOk;
	size_t program_size, log_size;
	int err;
	const char options[]= "-cl-fast-relaxed-math";

	/* Create program from file */
	program = clCreateProgramWithSource(context, 1, (const char**)&program_buffer, NULL, &err);
	if(err < 0) {
		perror("Couldn't create the program");
		exit(1);
	}

	/* Build program */
	err = clBuildProgram(program, 0, NULL, options, NULL, NULL);
	if(err < 0) {

		/* Find size of log and print to std output */
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 
			0, NULL, &log_size);
		program_log = (char*) malloc(log_size + 1);
		program_log[log_size] = '\0';
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 
			log_size + 1, program_log, NULL);
		printf("%s\n", program_log);
		free(program_log);
		exit(1);
	}
	return program;
}


