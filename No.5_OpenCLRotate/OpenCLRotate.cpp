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
	uchar *img_buffer, *out_buffer;

	// 1.定义输入文件和输出文件
	const char *img_file = "/data/local/tmp/ghost_yuv420_3264x2448.yuv";
	const char *out_file = "/data/local/tmp/rotate_output.yuv";

	// 2.分别为两个文件预分配缓冲区
	img_buffer = (uchar*)malloc(size);
	out_buffer = (uchar*)malloc(size);
	if ((NULL == img_buffer) || (NULL == out_buffer)) {
		perror("malloc memory fail");
		exit(EXIT_FAILURE);
	}

	// 3.将输入文件保存到缓冲区中
	load_data(img_file, img_buffer, width, height);

	// 4.旋转图像
	rotate(img_buffer, out_buffer, width, height);

	// 5.把旋转后的图像数据保存到输出文件中
	store_data(out_file, out_buffer, height, width);

	// 6.释放缓冲区
	free(img_buffer);
	free(out_buffer);

	return 0;
}
