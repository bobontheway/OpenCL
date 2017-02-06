#include "opencl.h"
#include "common.h"


/* Find a GPU or CPU associated with the first available platform */
cl_device_id create_device() {

	cl_platform_id platform;
	cl_device_id device;
	int err;

	/* Identify a platform */
	/*
	err = clGetPlatformIDs(1, &platform, NULL);
	if(err < 0) {
		perror("Couldn't identify a platform");
		
		*/

	//add
	   char* ext_data;							
   size_t ext_size;  
      cl_platform_id *platforms;
   cl_uint num_platforms;
   int i;

      //err = clGetPlatformIDs(1, NULL, &num_platforms);
      err = clGetPlatformIDs(0, NULL, &num_platforms);
   if(err < 0) {		
      perror("Couldn't find any platforms.");			
      exit(1);							
   }									
	printf("platform num: %d\n", num_platforms);
  
   /* Access all installed platforms */
   platforms = (cl_platform_id*) 					
      malloc(sizeof(cl_platform_id) * num_platforms);		
   clGetPlatformIDs(num_platforms, platforms, NULL);	

   for(i=0; i<num_platforms; i++) {
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


	/* Get device info - @s*/
	cl_uint d_num;
	err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_ALL, 0, NULL, &d_num);
	if(err < 0) {
		perror("Couldn't access any devices");
		exit(1);   
	}
	printf("device num: %d\n", d_num);


	/* @e */

	/* Access a device */
	err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	if(err == CL_DEVICE_NOT_FOUND) {
		err = clGetDeviceIDs(platforms[0], CL_DEVICE_TYPE_CPU, 1, &device, NULL);
	}
	if(err < 0) {
		perror("Couldn't access any devices");
		exit(1);   
	}

	//
	clGetDeviceInfo(device, CL_DEVICE_NAME, 0, NULL, &ext_size);
	ext_data = (char*)realloc(ext_data, ext_size);
	clGetDeviceInfo(device, CL_DEVICE_NAME, ext_size, ext_data, 0);
	printf("Device name: %s\n", ext_data);
	free(ext_data);
	//

	return device;
}

#if 1
void save_program_binary(cl_program program, cl_device_id device, const char* fileName)
{
	cl_uint numDevices = 0;
	cl_int errNum;
	cl_device_id *devices,*p;
	size_t *programBinarySizes;
	unsigned char **programBinaries;
	cl_uint i;
	FILE *fp;

	// 1 - Query for number of devices attached to program
	errNum = clGetProgramInfo(program, CL_PROGRAM_NUM_DEVICES, sizeof(cl_uint),
		&numDevices, NULL);
	if (errNum != CL_SUCCESS)
	{
		perror("Error querying for number of devices.\n");
		exit(1);
	}

	// 2 - Get all of the Device IDs

	devices = (cl_device_id*)malloc(sizeof(cl_device_id) * numDevices);
	if(device==NULL) exit(1);

	errNum = clGetProgramInfo(program, CL_PROGRAM_DEVICES,sizeof(cl_device_id) * numDevices, devices, NULL);
	if (errNum != CL_SUCCESS)
	{
		perror("Error querying for devices.\n");
		free(devices);
		exit(1);
	}
	// 3 - Determine the size of each program binary
	programBinarySizes =(size_t *)malloc(sizeof(size_t)*numDevices);
	if(programBinarySizes==NULL) exit(1);

	errNum = clGetProgramInfo(program, CL_PROGRAM_BINARY_SIZES,sizeof(size_t) * numDevices,programBinarySizes, NULL);
	if (errNum != CL_SUCCESS)
	{
		perror("Error querying for program binary sizes.\n");
		free(devices);
		free(programBinarySizes);
		exit(1);
	}

	programBinaries =(unsigned char**)malloc(sizeof(unsigned char*)*numDevices);
	if(programBinaries==NULL) exit(1);

	for (i = 0; i < numDevices; i++)
	{
		programBinaries[i] =(unsigned char *)malloc(programBinarySizes[i]);
	}

	// 4 - Get all of the program binaries
	errNum = clGetProgramInfo(program, CL_PROGRAM_BINARIES, sizeof(unsigned char*) * numDevices,
		programBinaries, NULL);
	if (errNum != CL_SUCCESS)
	{
		perror("Error querying for program binaries\n");

		free(devices);
		free(programBinarySizes);
		for (i = 0; i < numDevices; i++)
		{
			free(programBinaries[i]);
		}
		free(programBinaries);
		exit(1);
	}

	// 5 - Finally store the binaries for the device requested out to disk for future reading.
	for (i = 0; i < numDevices; i++)
	{
		// Store the binary just for the device requested.  In a scenario where
		// multiple devices were being used you would save all of the binaries out here.
		if (devices[i] == device)
		{
			fp = fopen(fileName, "wb");
			fwrite(programBinaries[i], 1, programBinarySizes[i], fp);
			fclose(fp);
			break;
		}
	}

	// Cleanup
	free(devices);
	free(programBinarySizes);
	for (i = 0; i < numDevices; i++)
	{
		free(programBinaries[i]);
	}
	free(programBinaries);
}
#endif

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
	//err = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	err = clBuildProgram(program, 0, NULL, options, NULL, NULL);
	//err = clBuildProgram(program, 1,&device,options, NULL, NULL);
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
	//save_program_binary(program,device,BINARY_FILE);
	return program;
}

#if 1
cl_program create_program_from_binary(cl_context context, cl_device_id device, const char* fileName)
{

	FILE *fp;
	size_t binarySize;
	unsigned char *programBinary;
	cl_int errNum = 0;
	cl_program program;
	cl_int binaryStatus;
	char buildLog[16384];
	const char options[]= "-cl-fast-relaxed-math";
	fp= fopen(fileName, "rb");
	if (fp == NULL)
	{
		return NULL;
	}

	// Determine the size of the binary

	fseek(fp, 0, SEEK_END);
	binarySize = ftell(fp);
	rewind(fp);

	programBinary =(unsigned char *)malloc(binarySize);
	fread(programBinary, 1, binarySize, fp);
	fclose(fp);

	program = clCreateProgramWithBinary(context,
		1,
		&device,
		&binarySize,
		(const unsigned char**)&programBinary,
		&binaryStatus,
		&errNum);
	free(programBinary);
	if (errNum!=0)
	{
		perror("Error loading program binary.\n");
		exit(1);
	}

	if (binaryStatus != 0)
	{
		perror("Invalid binary for device.\n");
		exit(1);
	}
	
	errNum = clBuildProgram(program, 0, NULL, options, NULL, NULL);
	if (errNum != CL_SUCCESS)
	{
	// Determine the reason for the error
	clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG,
	sizeof(buildLog), buildLog, NULL);
	perror("Error in program: \n");
	printf("%s\n",buildLog);
	clReleaseProgram(program);
	exit(1);
	}
	
	return program;
}
#endif

void make_size(size_t *global_size,size_t *local_size,size_t size)
{
	//size_t *local_size=(size_t *)malloc(2*sizeof(size_t));
	//size_t local_size[2];
	//size_t a,b;
	local_size[0]=global_size[0];
	if(global_size[0]<=size)
	{

		local_size[1]=size/local_size[0];
		//printf("%d,%d",local_size[0],local_size[1]);
			while(0!=(global_size[1]%local_size[1]))
		local_size[1]>>=1;
			//printf("%d,%d",local_size[0],local_size[1]);
	}
	else
	{
		while(local_size[0]>size)
		{
			//printf("%d,%d",local_size[0],local_size[1]);
			if(0==(local_size[0]&1))
				local_size[0]=local_size[0]>>1;
			else 
				local_size[0]=1;

		}
		local_size[1]=1;

	}
}


