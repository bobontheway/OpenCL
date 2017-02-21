#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <FreeImage.h>

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
		perror("bitmap load fail");
		exit(EXIT_FAILURE);
	}

	unsigned width = FreeImage_GetWidth(bitmap),
		 height = FreeImage_GetHeight(bitmap);

	printf("nWidth=%u nHeight=%u\n", width, height);
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

	const char *image_name = "image/android.png";
	const char *file_name = "android_rgba.bin";
	const char *dst_name = "dst_graphics.png";

	ret = load_image(image_name, file_name);
	if (ret) {
		printf("load image file fail\n");
		exit(EXIT_FAILURE);
	}

	ret = store_image(file_name, dst_name);
	if (ret) {
		printf("store image file fail\n");
		exit(EXIT_FAILURE);
	}


	return 0;
}
