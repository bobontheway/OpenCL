#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <FreeImage.h>

#include "util.h"

FREE_IMAGE_FORMAT g_format = (FREE_IMAGE_FORMAT)13;
int g_width = 512, g_height = 512;

/**
 * 将图像文件保存到二进制文件中
 */
int load_image(const char *image_name, const char *binary_name)
{
	int ret;
	FIBITMAP *bitmap, *bitmap_temp;

	// 获取位图格式，用于加载位图文件
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(image_name);
	//g_format = format;

	// 加载位图文件
	bitmap = FreeImage_Load(format, image_name);
	if (bitmap == NULL) {
		perror("bitmap load fail");
		exit(EXIT_FAILURE);
	}

	// 将位图转换为 32 位格式，返回的位图对象拷贝了输入位图
	bitmap_temp = bitmap;
	bitmap = FreeImage_ConvertTo32Bits(bitmap);
	FreeImage_Unload(bitmap_temp);

	unsigned width = FreeImage_GetWidth(bitmap),
		 height = FreeImage_GetHeight(bitmap);

#if 1
	printf("nWidth=%u nHeight=%u nFormat=%d\n", width, height, format);
#endif
	//g_width = width;
	//g_height = height;
	size_t size = width * height * 4; /* RGBA format */
	uint8_t *buffer = (unsigned char *)malloc(size);
	if (buffer == NULL) {
		printf("alloc memory fail\n");
		exit(EXIT_FAILURE);
	}

	// 返回指向数据位的指针
	memcpy(buffer, FreeImage_GetBits(bitmap), size);
	ret = store_data(binary_name, buffer, size);
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
	size_t size = g_width * g_height * 4;

	uint8_t *buffer = (unsigned char *)malloc(size);
	if (buffer == NULL) {
		printf("alloc memory fail\n");
		return EXIT_FAILURE;
	}

	ret = load_data(binary_name, buffer, size);
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
	const char *file_name = "lenna_rgba.bin";
	const char *dst_name = "lenna_target.png";

#if 0
	ret = load_image(image_name, file_name);
	if (ret) {
		printf("load image file fail\n");
		exit(EXIT_FAILURE);
	}
#endif

	ret = store_image(file_name, dst_name);
	if (ret) {
		printf("store image file fail\n");
		exit(EXIT_FAILURE);
	}


	return 0;
}
