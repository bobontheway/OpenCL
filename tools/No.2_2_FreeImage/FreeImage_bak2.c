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

#if 0
void GPU_Rotate(char *ImageFileName)
{
	cl_platform_id platform;
	cl_device_id device;
	cl_context context;
	cl_command_queue queue;
	cl_program program;
	cl_kernel kernel;
	cl_mem imageObjects[2];
	cl_int err;

	err = clGetPlatformIDs(1, &platform, NULL);
	//err = clGetDevceiIDs(platform, CL_DEVICE_TYPE_CPU, 1, &device, NULL);
	err = clGetDevceiIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
	context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
	command = clCreateCommandQueue(context, device, NULL, &err);

	cl_mem imageObjects[2];
	int width, height;
	// 加载原始图像
	imageObjects[0] = LoadImage(context, ImageFileName, width, height);

	// 创建输出图像对象
	cl_image_format clImageFormat;
	clImageFormat.image_channel_order = CL_RGBA;
	clImageFormat.image_channel_data_type = CL_UNORM_INT8;

	cl_image_desc desc;
	memset(&desc, '\0', sizeof(cl_image_desc));
	desc.image_height = height;
	desc.image_width = width;
	desc.image_type = CL_MEM_OBJECT_IMAGE2D;
	imageObjects[1] = clCreateImage(context, CL_MEM_WRITE_ONLY,
		&clImageFormat, &desc, NULL, &err);

	program = CreateProgram(context, device, "image_rotate.cl");
	kernel = clCreateKernel(program, "image_rotate", &err);
	err = clSetKernelArg(kernel, 0, sizeof(cl_mem), &imageObjects[0]);
	err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &imageObjects[1]);
	float angel = 45 * PI / 180.0f;
	err |= clSetKernelArg(kernel, 2, sizeof(cl_float), &angle);

	size_t globalSize[2] = {width, height};
	err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, globalSzie, NULL,
		0, NULL, NULL);
	clFinish(queue);

	char *buffer = malloc(width * height * 4);
	size_t origin[3],
	       reginon[3] = {width, hegith, 1};
	err = clEnqueueReadImage(queue, imageObjects[1], CL_TRUE, origin,
		region, 0, 0, buffer, 0, NULL, NULL);
	SaveImage("gpu_rotate.bmp", buffer, width, height);
	Cleanup(context, queue, program, kernel, imageObjects);



}
#endif

int main()
{
	FIBITMAP *bitmap;

	// 获取图像的格式，用于图像加载
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType("orig_graphics.png");

	// 加载图像
	bitmap = FreeImage_Load(format, "orig_graphics.png");
	if (bitmap == NULL) {
		perror("bitmap load fail");
		exit(EXIT_FAILURE);
	}

	// 将位图转换为 32 位，返回的位图对象拷贝了输入位图对应的数据
	bitmap = FreeImage_ConvertTo32Bits(bitmap);

#if 0
	// 旋转，图像大小会发生变化
	bitmap = FreeImage_Rotate(bitmap, 30.0);
#endif
	// 旋转，图像大小不变
	double x_orig = FreeImage_GetWidth(bitmap) / (double)2;
	double y_orig = FreeImage_GetHeight(bitmap) / (double)2;
	// 先旋转，再平移
	bitmap = FreeImage_RotateEx(bitmap, 30.0, 0, 0, x_orig, y_orig, TRUE);

	// 转换为 bmp 类型
	bitmap = FreeImage_ConvertToType(bitmap, FIT_BITMAP); // FREE_IMAGE_TYPE
	FreeImage_Save(format, bitmap, "dst_graphics.png");

	printf("Hello world\n");

#if 0
	//
	unsigned width = FreeImage_GetWidth(bitmap),
		 height = FreeImage_GetHeight(bitmap);

	printf("nWidth=%u nHeight=%u\n", width, height);
	char *buffer = (char *)malloc(width * height * 4);
	if (buffer == NULL) {
		printf("alloc memory fail\n");
		exit(-1);
	}

	// 返回指向数据位的指针
	memcpy(buffer, FreeImage_GetBits(bitmap), width * height * 4);
	FreeImage_Unload(bitmap);
#endif

#if 0
	cl_image_format clImageFormat;
	clImageFormat.image_channel_order = CL_RGBA;
	clImageFormat.image_channel_data_type = CL_UNORM_INT8;

	cl_image_desc clImageDesc;
	memset(&clImageDesc, 0 sizeof(cl_image_format));
	clImageDesc.image_type = CL_MEM_OBJECT_IMAGE2D;
	clImageDesc.image_width = width;
	clImageDesc.image_height = height;

	cl_int errNum;
	cl_mem clImage;
	clImage = clCreateImage(context, CL_MEM_READ_ONLY |
		CL_MEM_COPY_HOST_PTR, &clImageFormat, &clImageDesc,
		buffer, &errNum);
#endif


#if 0
	// 获取 FreeImage 版本信息
	const char *ver = FreeImage_GetVersion();
	const char *copyr = FreeImage_GetCopyrightMessage();
	printf("version:%s\n", ver);
	printf("copyRight:%s\n", copyr);
#endif
}
