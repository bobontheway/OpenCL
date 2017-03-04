## 简介
该程序基于 [No.6_2_OpenCLSampler](../No.6_2_OpenCLSampler/OpenCLSampler.md)，功能保持不变，对图像执行旋转操作。不同的是在 Ubuntu 下运行，GPU 为 AMD RX 460，支持 OpenCL 1.2 版本。

## 实现
本程序在 No.6_2_OpenCLSampler 的基础上做了一些修改，改进如下：
- 使用 clCreateImage 来创建图像对象；
- 增加对 FreeImage 的接口调用；
- 如果采样点的区域在图像外，将颜色设置为黑色。

### 1.创建图像对象
```c
cl_mem in_buffer;

cl_image_desc image_desc;
memset((void *)&image_desc, 0, sizeof(cl_image_desc));
image_desc.image_type = CL_MEM_OBJECT_IMAGE2D;
image_desc.image_width = w;
image_desc.image_height = h;

cl_image_format image_format;
memset((void *)&image_format, 0, sizeof(cl_image_format));
image_format.image_channel_order = CL_RGBA;
image_format.image_channel_data_type = CL_UNORM_INT8;

in_buffer = clCreateImage(context,
	CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, &image_format,
	&image_desc, src, &err);
```
和 OpenCL 1.1 中的 `clCreateImage2D` 函数不同，`clCreateImage` 将图像的属性，例如宽、高等信息放到 `cl_image_desc` 中，统一配置。

### 2.FreeImage 图形库
由于 FreeImage 开原图形库已增加了对 Ubuntu 系统支持，该程序中可直接调用其接口，无需再使用 [No.2_2_FreeImage](../tools/No.2_2_FreeImage/FreeImage.md) 工具执行位图文件和图像数据之间的转换。[No.2_1_FreeImage](../tools/No.2_1_FreeImage/FreeImage.md) 示例中包含了对 FreeImage 接口的使用说明。

### 3.内核代码
由于采样器的寻址模式为 `CLK_ADDRESS_CLAMP`，超过图像区域的坐标返回边框的颜色，CL_RGBA 边框颜色中 Alpha 通道对应的透明为全透明。为了看起来舒服一些，在内核代码实现时，如果采样点的区域在图像外，将颜色设置为黑色。相关代码如下：
```c
__constant  sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE |
	CLK_FILTER_NEAREST | CLK_ADDRESS_CLAMP;

__kernel void rotate_rgba(__read_only image2d_t srcImg,
	__write_only image2d_t dstImg,
	float angle)
{
	int width = get_image_width(srcImg);
	int height = get_image_height(srcImg);
	const int x = get_global_id(0);
	const int y = get_global_id(1);
	float sinma = sin(angle);
	float cosma = cos(angle);

	// 计算旋转点中心
	int ox = width / 2;
	int oy = height / 2;

	// 相对于圆心的点，临时变量（将圆心看做坐标原点）
	int xt = x - ox;
	int yt = y - oy;

	// 计算旋转后坐标。y 是右上方上面的点（顺时针旋转）
	float2 readCoord;
	readCoord.x = (cosma * xt - sinma * yt) + ox;
	readCoord.y = (sinma * xt + cosma * yt) + oy;

	// 根据旋转后坐标读取元素图像元素值
	float4 value = read_imagef(srcImg, sampler, readCoord);
	if ((readCoord.x > get_global_size(0) || readCoord.y >
		get_global_size(1)) || (readCoord.x < -0.0f ||
		readCoord.y < -0.0f))
		write_imagef(dstImg, (int2)(x, y), (float4)(0.0, 0.0, 0.0, 1.0));
	else
		write_imagef(dstImg, (int2)(x, y), value);
}
```

## 结果
该程序运行后，显示效果如下图所示。

**原始图像**

<img src="image/lenna.png" width="40%" height="40%">

**旋转后图像**

<img src="image/lenna_target.png" width="40%" height="20%">
