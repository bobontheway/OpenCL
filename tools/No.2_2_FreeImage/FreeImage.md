## 简介
FreeImage 是一款免费的开源图形库，它可以用于 Windows，Linux 和 Mac OS X 系统。本文基于 [No.2_1_FreeImage](../No.2_1_FreeImage/FreeImage.md)，在 Android 系统下通过调用 `OpenCL API` 执行旋转操作。由于 Android 系统不支持 FreeImage，[No.6_1_OpenCLSampler](../../No.6_1_OpenCLSampler/OpenCLSampler.md) 在对图像做旋转操作时，先在 Ubuntu 中通过调用 FreeImage 库函数将位图文件转换为图像数据保存在二进制文件中。然后在 Android 系统中加载该二进制文件，调用 OpenCL API 旋转图像，将旋转后的图像也保存为二进制文件。最后在 Ubuntu 中加载旋转后的二进制文件，通过 FreeImage 接口将其存储为位图文件，由此得到旋转后的位图文件。具体执行流程如下图所示，Android 中执行 OpenCL 代码参见 []()：

**执行流程**

![](image/lenna.png)

## 实现
该程序实现了两个函数 `load_image` 和 `store_image`，分别用于位图文件的加载和保存。

### 1.加载位图文件
加载位图文件，并将其保存为二进制文件。获取位图文件的格式，然后根据格式将其加载到内存中。接着将位图对象转换为 32 位字节流格式，提取对应的比特位，并将其保存为二进制文件。

这里使用的 FreeImage 库中部分函数在 [No.6_1_OpenCLSampler](../No.6_1_OpenCLSampler/OpenCLSampler.md) 中已有描述，不再赘述。只对 FreeImage_GetBits 做简要描述：
```c
FIBITMAP *bitmap;
BYTE *bits = FreeImage_GetBits(bitmap);
```
返回指向位图对象 `bitmap` 对应数据位的指针。如果位图对象没有包含像素数据，该函数返回 NULL。通过更新比特位数据，`FreeImage_GetBits` 函数可用于更改图像，下面保存位图文件就使用了这种方式。

### 2.保存位图文件
加载二进制文件，并将其保存为位图文件。将二进制文件加载到内存中，然后创建位图对象。接着获取指向位图对象的字节流指针，将内存中的二进制内容更新到位图对象对应的字节流，最后保存位图文件。相关函数在前面已有描述，这里只对 `FreeImage_Allocate` 做简要描述：
```c
FIBITMAP *bitmap = FreeImage_Allocate(g_width, g_height, 32);
```
创建位图对象，并为对应的图像数据分配内存。位图中数据初始化为 0，默认黑色。`width` 和 `height` 分别表示图像的宽度和高度，以像素为单位。`bpp` 表示每个像素的深度，即占用内存的大小。如果成功执行，该函数返回位图对象 `FIBITMAP`。
