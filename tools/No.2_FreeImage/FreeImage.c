#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <FreeImage.h>

#include "util.h"

int main()
{
	const char *image_name = "image/android.png";
	const char *file_name = "android_rgba.bin";
	FIBITMAP *bitmap;
	int ret;

	// 获取图像的格式，用于图像加载
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(image_name);

	// 加载图像
	bitmap = FreeImage_Load(format, image_name);
	if (bitmap == NULL) {
		perror("bitmap load fail");
		exit(EXIT_FAILURE);
	}

	unsigned width = FreeImage_GetWidth(bitmap),
		 height = FreeImage_GetHeight(bitmap);

	printf("nWidth=%u nHeight=%u\n", width, height);

	char *buffer = (char *)malloc(width * height * 4);
	if (buffer == NULL) {
		printf("alloc memory fail\n");
		exit(EXIT_FAILURE);
	}

	// 返回指向数据位的指针
	memcpy(buffer, FreeImage_GetBits(bitmap), width * height * 4);
	ret = store_data(file_name, buffer, width * height * 4);
	if (ret) {
		printf("store data to file fail: %s\n", file_name);
	}

	FreeImage_Unload(bitmap);
	free(buffer);
	return 0;
}
