#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#include "rotate.h"

int main()
{
	uint width = 3264,
	     height = 2448,
	     size = width * height * 3/2;
	uint8_t *img_buffer;
	uint8_t *out_opencl, *out_normal, *out_shift,
		*out_delete_shift, *out_opencl_use;

	// 1.定义输入文件和输出文件
	const char *img_file = "/data/local/tmp/ghost_yuv420_3264x2448.yuv";
	const char *out_opencl_file = "/data/local/tmp/rotate_out_opencl.yuv";
	const char *out_normal_file = "/data/local/tmp/rotate_out_normal.yuv";
	const char *out_shift_file = "/data/local/tmp/rotate_out_shift.yuv";
	const char *out_delete_shift_file =
		"/data/local/tmp/rotate_out_delete_shift.yuv";
	const char *out_opencl_use_file =
		"/data/local/tmp/rotate_out_opencl_use.yuv";

	// 2.分别为两个文件预分配缓冲区
	img_buffer = (uint8_t *)malloc(size);
	out_opencl = (uint8_t *)malloc(size);
	out_normal = (uint8_t *)malloc(size);
	out_shift = (uint8_t *)malloc(size);
	out_delete_shift = (uint8_t *)malloc(size);
	out_opencl_use = (uint8_t *)malloc(size);
	if (!img_buffer || !out_opencl || !out_normal || !out_shift ||
		!out_delete_shift || !out_opencl_use) {
		perror("malloc memory fail");
		exit(EXIT_FAILURE);
	}

	// 3.将输入文件保存到缓冲区中
	load_data(img_file, img_buffer, width, height);

	// 4.旋转图像
	rotate(img_buffer, out_opencl, out_normal, out_shift,
		out_delete_shift, out_opencl_use, width, height);

	// 5.把旋转后的图像数据保存到输出文件中
	store_data(out_opencl_file, out_opencl, height, width);
	store_data(out_normal_file, out_normal, height, width);
	store_data(out_shift_file, out_shift, height, width);
	store_data(out_delete_shift_file, out_delete_shift, height, width);
	store_data(out_opencl_use_file, out_opencl_use, height, width);

	// 6.释放缓冲区
	free(img_buffer);
	free(out_opencl);
	free(out_normal);
	free(out_shift);
	free(out_delete_shift);
	free(out_opencl_use);

	return 0;
}
