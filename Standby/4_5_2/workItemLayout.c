#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
//#include <OpenCL/opencl.h>
#include <CL/cl.h>

////////////////////////////////////////////////////////////////////////////////

// Use a static data size for simplicity
//
#define DATA_SIZE (1024)

////////////////////////////////////////////////////////////////////////////////

// Simple compute kernel which computes the square of an input array
const char *KernelSource =
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
"}                                                                              \n"

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char** argv)
{
    int err;                            // error code returned from api calls
    
    float data[DATA_SIZE];              // original data set given to device
    float results[DATA_SIZE];           // results returned from device
    unsigned int correct;               // number of correct results returned
    
    size_t global;                      // global domain size for our calculation
    size_t local;                       // local domain size for our calculation
    
    cl_device_id device_id;             // compute device id
    cl_context context;                 // compute context
    cl_command_queue commands;          // compute command queue
    cl_program program;                 // compute program
    cl_kernel kernel;                   // compute kernel
    
    cl_mem input;                       // device memory used for the input array
    cl_mem output;                      // device memory used for the output array
    
    printf("[Base everonment information]\n");
    
    // OpenCL platform
    cl_platform_id *platform;
    cl_uint num_platform;
    
    err = clGetPlatformIDs(0, NULL, &num_platform);
    printf("num_platform_id=%d\n", (int)num_platform);
    
    platform = (cl_platform_id *)malloc(sizeof(cl_platform_id) * num_platform);
    err = clGetPlatformIDs(num_platform, platform, NULL);
    
    size_t size;
    err = clGetPlatformInfo(platform[0], CL_PLATFORM_PROFILE, 0, NULL, &size);
    char *PProfile = (char *)malloc(size);
    err = clGetPlatformInfo(platform[0], CL_PLATFORM_PROFILE, size, PProfile, 0);
    printf("CL_PLATFORM_PROFILE=%s\n", PProfile );
    
    err = clGetPlatformInfo(platform[0], CL_PLATFORM_VERSION, 0, NULL, &size);
    char *PVersion = (char *)malloc(size);
    err = clGetPlatformInfo(platform[0], CL_PLATFORM_VERSION, size, PVersion, 0);
    printf("CL_PLATFORM_VERSION=%s\n", PVersion);
    
    err = clGetPlatformInfo(platform[0], CL_PLATFORM_NAME, 0, NULL, &size);
    char *PName  = (char *)malloc(size);
    err = clGetPlatformInfo(platform[0], CL_PLATFORM_NAME, size, PName, 0);
    printf("CL_PLATFORM_NAME=%s\n", PName);
    
    err = clGetPlatformInfo(platform[0], CL_PLATFORM_VENDOR, 0, NULL, &size);
    char *PVendor  = (char *)malloc(size);
    err = clGetPlatformInfo(platform[0], CL_PLATFORM_VENDOR, size, PVendor, 0);
    printf("CL_PLATFORM_VENDOR=%s\n", PVendor);
    
    err = clGetPlatformInfo(platform[0], CL_PLATFORM_EXTENSIONS, 0, NULL, &size);
    char *PExtensions  = (char *)malloc(size);
    err = clGetPlatformInfo(platform[0], CL_PLATFORM_EXTENSIONS, size, PExtensions, 0);
    printf("CL_PLATFORM_EXTENSIONS=%s\n\n", PExtensions);
    
    free(PProfile);
    free(PVersion);
    free(PName);
    free(PVendor);
    free(PExtensions);
    
    // OpenCL device
    cl_device_id *devices;
    cl_uint num_device;
    
    err = clGetDeviceIDs(platform[0], CL_DEVICE_TYPE_GPU, 0, NULL, &num_device);
    printf("num_device_id=%d\n", (int)num_device);
    
    devices = (cl_device_id *)malloc(sizeof(cl_device_id) * num_device);
                                     
    
    err = clGetDeviceIDs(platform[0], CL_DEVICE_TYPE_GPU, num_device, devices, NULL);
    for (int i = 0; i < num_device; i++) {
        char buffer[100];
        err = clGetDeviceInfo(devices[i], CL_DEVICE_NAME, 100, buffer, NULL);
        printf("Device Name:%s\n", buffer);
        
        cl_uint UnitNum;
        err = clGetDeviceInfo(devices[i], CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(cl_uint), &UnitNum, NULL);
        printf("Compute Units Number: %d\n", UnitNum);
        
        cl_uint freq;
        err = clGetDeviceInfo(devices[i], CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(cl_uint), &freq, NULL);
        printf("Device Frequency: %d(MHz)\n", freq);
        
        cl_ulong globalSize;
        err = clGetDeviceInfo(devices[i], CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &globalSize, NULL);
        printf("Device Global Size: %0.0f(MB)\n", (float)globalSize / 1024 / 1024);
        
        cl_uint globalCacheLine;
        err = clGetDeviceInfo(devices[i], CL_DEVICE_GLOBAL_MEM_CACHELINE_SIZE, sizeof(cl_uint), &globalCacheLine, NULL);
        printf("Device Global CacheLine: %d(Byte)\n", globalCacheLine);
        
        err = clGetDeviceInfo(devices[i], CL_DEVICE_VERSION, 100, buffer, NULL);
        printf("Device Version:%s\n", buffer);
        
        char *deviceExtensions;
        size_t extenNum;
        err = clGetDeviceInfo(devices[i], CL_DEVICE_EXTENSIONS, 0, NULL, &extenNum);
        deviceExtensions = (char *)malloc(extenNum);
        err = clGetDeviceInfo(devices[i], CL_DEVICE_EXTENSIONS, extenNum, deviceExtensions, NULL);
        printf("Device Extensions:%s\n", deviceExtensions);
        free(deviceExtensions);
        
        
        printf("\n");
    }
        
    return 0;
}


