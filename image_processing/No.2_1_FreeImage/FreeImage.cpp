#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <FreeImage.h>

void get_image_property(FIBITMAP *bitmap)
{
	unsigned width = FreeImage_GetWidth(bitmap);
	unsigned height = FreeImage_GetHeight(bitmap);
	printf("width=%u height=%u\n", width, height);
}

int main()
{
	const char *image_name = "image/lenna.png";
	FIBITMAP *bitmap, *bitmap_temp;

	// 获取位图格式，用于加载位图文件
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(image_name);

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
#if 0
	// 旋转，图像大小会发生变化
	bitmap = FreeImage_Rotate(bitmap, 30.0);
#endif
	// 旋转，图像大小不变
	double x_orig = FreeImage_GetWidth(bitmap) / (double)2;
	double y_orig = FreeImage_GetHeight(bitmap) / (double)2;

	// 先旋转，再平移
	bitmap_temp = bitmap;
	bitmap = FreeImage_RotateEx(bitmap, 30.0, 0, 0, x_orig, y_orig, TRUE);
	FreeImage_Unload(bitmap_temp);

	// 转换为 bmp 类型，存入文件中
	bitmap_temp = bitmap;
	bitmap = FreeImage_ConvertToType(bitmap, FIT_BITMAP); // FREE_IMAGE_TYPE
	FreeImage_Unload(bitmap_temp);

	FreeImage_Save(format, bitmap, "lenna_target.png");
	FreeImage_Unload(bitmap);
}
