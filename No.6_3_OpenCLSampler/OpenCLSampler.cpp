#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#include "util.h"

extern void scissor(uint8_t *src, uint8_t *des, int orig_width, int orig_height,
	int new_width, int new_height);

int main()
{
	/* RGBA image format */
	uint orig_width = 512,
	     orig_height = 512,
	     size = orig_width * orig_height * 4;

	uint new_width = 256,
	     new_height = 256;

	uint8_t *img_buffer;
	uint8_t *out_buffer;

	// 1.定义输入文件和输出文件
	const char *img_file = "/data/local/tmp/lenna_rgba.bin";
	const char *out_file = "lenna_rgba_target.bin";

	// 2.分别为两个文件预分配缓冲区
	img_buffer = (uint8_t *)malloc(size);
	out_buffer = (uint8_t *)malloc(size);
	if (!img_buffer || !out_buffer) {
		perror("malloc memory fail");
		exit(EXIT_FAILURE);
	}

	// 3.将输入文件保存到缓冲区中
	load_data(img_file, img_buffer, size);

	// 4.旋转图像
	scissor(img_buffer, out_buffer, orig_width, orig_height,
		new_width, new_height);

	// 5.把旋转后的图像数据保存到输出文件中
	store_data(out_file, out_buffer, 384 * 384 * 4);

	// 6.释放缓冲区
	free(img_buffer);
	free(out_buffer);

	return 0;
}
