#ifndef __OPENCL_H__
#define __OPENCL_H__

#ifdef MAC
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#define HIST_LOCAL_KERNEL "hist_local"
#define HIST_GLOBAL_KERNEL "hist_global"
#define XPROJ_KERNEL "xproj"
#define YPROJ_KERNEL "yproj"
#define XSAD_KERNEL "xsad"
#define YSAD_KERNEL "ysad"
#define AFFINE_UV_KERNEL "affine_uv"
#define FUSION51_1_KERNEL "tempMask"
#define FUSION51_2_KERNEL "blurMask"
#define FUSION51_3_KERNEL "fusionY51"
#define FUSION41_1_KERNEL "fusionY41"



cl_device_id create_device();
cl_program build_program(cl_context context, cl_device_id device, const char* filename1, const char* filename2);
void save_program_binary(cl_program program, cl_device_id device, const char* fileName);
cl_program create_program_from_binary(cl_context context, cl_device_id device, const char* fileName);
void make_size(size_t *global_size,size_t *local_size,size_t size);

#endif

