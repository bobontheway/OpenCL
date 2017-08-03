#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <FreeImage.h>

FREE_IMAGE_FORMAT g_format;
int g_width, g_height;
	
/**
 * 将图像数据加载到缓冲区中
 */
int load_image(const char *image_name, uint8_t *buffer)
{
	int ret;
	FIBITMAP *bitmap, *bitmap_temp;

	// 获取图像的格式，用于图像加载
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(image_name);
	g_format = format;

	// 加载图像
	bitmap = FreeImage_Load(format, image_name);
	if (bitmap == NULL) {
		printf("bitmap load fail: %s\n", image_name);
		exit(EXIT_FAILURE);
	}

	// 将位图转换为 8 位灰度格式，返回的位图对象拷贝了输入位图
	bitmap_temp = bitmap;
	bitmap = FreeImage_ConvertTo8Bits(bitmap); // 转换为 8 位的灰度图像
	FreeImage_Unload(bitmap_temp);

	unsigned width = FreeImage_GetWidth(bitmap),
		 height = FreeImage_GetHeight(bitmap);

#if 0
	printf("nWidth=%u nHeight=%u nFormat=%d\n", width, height, format);
#endif
	g_width = width;
	g_height = height;

	// 返回指向数据位的指针
	memcpy(buffer, FreeImage_GetBits(bitmap), width * height);
	FreeImage_Unload(bitmap);

	return ret;
}

/**
 * 把缓冲区中的数据保存为图像文件
 */
int store_image(uint8_t *buffer, const char *image_name)
{
	int i, j, ret = 0;

	// 创建新的位图对象
	FIBITMAP *bitmap = FreeImage_Allocate(g_width, g_height, 32);
	if (bitmap == NULL) {
		printf("alloc bitmap fail\n");
		return EXIT_FAILURE;
	}

	uint8_t *bits = FreeImage_GetBits(bitmap);
	for (i = 0; i < g_width * g_height; i++) {
		for (j = 0; j < 3; j++)
			bits[i*4+j] = buffer[i];
		bits[i*4+3] = 255;
	}

	FreeImage_Save(g_format, bitmap, image_name);
	FreeImage_Unload(bitmap);

	return ret;
}

int main()
{

	int ret;
	const char *image_name = "image/lenna.png";
	const char *dst_name = "lenna_target.png";

	/* Bitmap format RGBA */
	uint width = 512,
	     height = 512,
	     gray_size = width * height,
	     size = width * height * 4;

	uint8_t *img_gray_buffer;
	uint8_t *out_buffer;

	// 1.为两个位图文件预分配缓冲区
	img_gray_buffer = (uint8_t *)malloc(gray_size);
	out_buffer = (uint8_t *)malloc(size);
	if (!img_gray_buffer || !out_buffer) {
		perror("malloc memory fail");
		exit(EXIT_FAILURE);
	}

	// 2.将图像数据加载到缓冲区
	ret = load_image(image_name, img_gray_buffer);
	if (ret) {
		printf("load image file fail: %s\n", image_name);
		exit(EXIT_FAILURE);
	}

	// 3.将 img_gray_buffer 缓冲区中的数据存储为图像文件
	ret = store_image(img_gray_buffer, dst_name);
	if (ret) {
		printf("store image file fail\n");
		exit(EXIT_FAILURE);
	}

	// 4.释放缓冲区
	free(img_gray_buffer);
	free(out_buffer);

	return 0;
}
