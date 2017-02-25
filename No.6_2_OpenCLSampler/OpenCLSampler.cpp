#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

#include <FreeImage.h>

#include "rotate.h"
#include "util.h"

FREE_IMAGE_FORMAT g_format;
int g_width, g_height;
	
/**
 * 将图像文件保存到二进制文件中
 */
int load_image(const char *image_name, const char *binary_name)
{
	int ret;
	FIBITMAP *bitmap;

	// 获取图像的格式，用于图像加载
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(image_name);
	g_format = format;

	// 加载图像
	bitmap = FreeImage_Load(format, image_name);
	if (bitmap == NULL) {
		printf("bitmap load fail: %s\n", image_name);
		exit(EXIT_FAILURE);
	}

	// 将位图转换为 32 位格式，返回的位图对象拷贝了输入位图对应的数据
	bitmap = FreeImage_ConvertTo32Bits(bitmap);

	unsigned width = FreeImage_GetWidth(bitmap),
		 height = FreeImage_GetHeight(bitmap);

	printf("nWidth=%u nHeight=%u nFormat=%d\n", width, height, format);
	g_width = width;
	g_height = height;

	uint8_t *buffer = (unsigned char *)malloc(width * height * 4);
	if (buffer == NULL) {
		printf("alloc memory fail\n");
		exit(EXIT_FAILURE);
	}

	// 返回指向数据位的指针
	memcpy(buffer, FreeImage_GetBits(bitmap), width * height * 4);
	ret = store_data(binary_name, buffer, width * height * 4);
	if (ret) {
		printf("store data to file fail: %s\n", binary_name);
		return EXIT_FAILURE;
	}

	FreeImage_Unload(bitmap);
	free(buffer);

	return ret;
}

/**
 * 把二进制文件中的数据保存为图像文件
 */
int store_image(const char *binary_name, const char *image_name)
{
	int i, ret = 0;

	uint8_t *buffer = (unsigned char *)malloc(g_width * g_height * 4);
	if (buffer == NULL) {
		printf("alloc memory fail\n");
		return EXIT_FAILURE;
	}

	ret = load_data(binary_name, buffer, g_width * g_height * 4);
	if (ret) {
		printf("load data from file fail: %s\n", binary_name);
		free(buffer);
		return EXIT_FAILURE;
	}

	// 创建新的位图对象
	FIBITMAP *bitmap = FreeImage_Allocate(g_width, g_height, 32);
	if (bitmap == NULL) {
		printf("alloc bitmap fail\n");
		free(buffer);
		return EXIT_FAILURE;
	}

	uint8_t *bits = FreeImage_GetBits(bitmap);
	for (i = 0; i < g_width * g_height * 4; i++)
		bits[i] = buffer[i];

	FreeImage_Save(g_format, bitmap, image_name);
	FreeImage_Unload(bitmap);
	free(buffer);

	return ret;
}

int main()
{

	int ret;

	const char *image_name = "image/lenna.png";
	const char *binary_name = "lenna_rgba.bin";
	const char *binary_rotated_name = "lenna_rotated_rgba.bin";
	const char *dst_name = "lenna_target.png";

	// 图像文件转换为二进制文件
	ret = load_image(image_name, binary_name);
	if (ret) {
		printf("load image file fail: %s\n", image_name);
		exit(EXIT_FAILURE);
	}


#if 1 // 将二进制文件旋转后存为二进制文件
	/* RGBA */
	uint width = 512,
	     height = 512,
	     size = width * height * 4;
	float angle = 30;

	uint8_t *img_buffer;
	uint8_t *out_buffer;

#if 0
	// 1.定义输入文件和输出文件
	const char *img_file = "/data/local/tmp/lenna_rgba.bin";
	const char *out_file = "lenna_rgba_target.bin";
#endif

	// 2.分别为两个文件预分配缓冲区
	img_buffer = (uint8_t *)malloc(size);
	out_buffer = (uint8_t *)malloc(size);
	if (!img_buffer || !out_buffer) {
		perror("malloc memory fail");
		exit(EXIT_FAILURE);
	}

	// 3.将输入文件保存到缓冲区中
	load_data(binary_name, img_buffer, size);

	// 4.旋转图像
	rotate(img_buffer, out_buffer, width, height, angle);

	// 5.把旋转后的图像数据保存到输出文件中
	store_data(binary_rotated_name, out_buffer, size);

	// 6.释放缓冲区
	free(img_buffer);
	free(out_buffer);
#endif

	// 将二进制文件转换为图像文件
	ret = store_image(binary_rotated_name, dst_name);
	if (ret) {
		printf("store image file fail\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}
