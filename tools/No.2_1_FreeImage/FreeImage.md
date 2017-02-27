该程序使用 FreeImage 来对图像执行旋转操作。FreeImage 是一款免费的开源图形库，它可以用于 Windows，Linux 和 Mac OS X 系统。

### 1.获取位图格式
```c
FREE_IMAGE_FORMAT format = FreeImage_GetFileType(image_name);
```
从位图文件获取位图格式。该函数从位图文件中读取 16 字节内容并对其分析，从而得到位图格式。返回值 `FREE_IMAGE_FORMAT` 在加载或存储位图文件时，用于标识位图格式，可以是下面的常量值：
FIF           | Description
--------------|------------
FIF_UNKNOWN   |Unknown format (returned value only, never use it as input value)
FIF_BMP       |Windows or OS/2 Bitmap File (*.BMP)
FIF_CUT       |Dr. Halo (*.CUT)
FIF_DDS       |DirectDraw Surface (*.DDS)
FIF_EXR       |ILM OpenEXR (*.EXR)
FIF_FAXG3     |Raw Fax format CCITT G3 (*.G3)
FIF_GIF       |Graphics Interchange Format (*.GIF)
FIF_HDR       |High Dynamic Range (*.HDR)
FIF_ICO       |Windows Icon (*.ICO)
FIF_IFF       |Amiga IFF (*.IFF, *.LBM)
FIF_J2K       |JPEG-2000 codestream (*.J2K, *.J2C)
FIF_JNG       |JPEG Network Graphics (*.JNG)
FIF_JP2       |JPEG-2000 File Format (*.JP2)
FIF_JPEG      |Independent JPEG Group (*.JPG, *.JIF, *.JPEG, *.JPE)
FIF_JXR       |JPEG XR image format (*.JXR, *.WDP, *.HDP)
FIF_KOALA     |Commodore 64 Koala format (*.KOA)
FIF_MNG       |Multiple Network Graphics (*.MNG)
FIF_PBM       |Portable Bitmap (ASCII) (*.PBM)
FIF_PBMRAW    |Portable Bitmap (BINARY) (*.PBM)
FIF_PCD       |Kodak PhotoCD (*.PCD)
FIF_PCX       |Zsoft Paintbrush PCX bitmap format (*.PCX)
FIF_PFM       |Portable Floatmap (*.PFM)
FIF_PGM       |Portable Graymap (ASCII) (*.PGM)
FIF_PGMRAW    |Portable Graymap (BINARY) (*.PGM)
FIF_PICT      |Macintosh PICT (*.PCT, *.PICT, *.PIC)
FIF_PNG       |Portable Network Graphics (*.PNG)
FIF_PPM       |Portable Pixelmap (ASCII) (*.PPM)
FIF_PPMRAW    |Portable Pixelmap (BINARY) (*.PPM)
FIF_PSD       |Adobe Photoshop (*.PSD)
FIF_RAS       |Sun Rasterfile (*.RAS)
FIF_RAW       |RAW camera image (many extensions)
FIF_SGI       |Silicon Graphics SGI image format (*.SGI)
FIF_TARGA     |Truevision Targa files (*.TGA, *.TARGA)
FIF_TIFF      |Tagged Image File Format (*.TIF, *.TIFF)
FIF_WBMP      |Wireless Bitmap (*.WBMP)
FIF_WEBP      |Google WebP image format (*.WEBP)
FIF_XBM       |X11 Bitmap Format (*.XBM)
FIF_XPM       |X11 Pixmap Format (*.XPM)

### 2.加载位图文件
```c
FIBITMAP *bitmap = FreeImage_Load(format, image_name);
if (bitmap == NULL) {
        perror("bitmap load fail");
        exit(EXIT_FAILURE);
}
```
加载位图文件，解析位图属性，并为位图分配内存，返回 FIBITMAP 对象。第一个参数定义被加载的位图类型，当传递 FIF_BMP 作为参数时，表示 BMP 位图文件被加载到内存；第二个参数指向被加载的位图文件位置。

### 3.格式转换
```c
bitmap = FreeImage_ConvertTo32Bits(bitmap);
```
将位图格式转换为 32 位字节流，返回的位图对象是输入位图的拷贝，已经转换为 32 位位图。实际上转换后的 32 位位图对象指向新的内存区域，该内存区域可能比原始位图指向的内存区域要大。由于后续操作使用 32 位位图对象，原来的位图对象需要使用 `FreeImage_Unload` 来释放资源。

### 4.旋转图像
```c
bitmap = FreeImage_RotateEx(bitmap, 30.0, 0, 0, x_orig, y_orig, TRUE);
```
对 8 位灰度，24 或 32 位图像执行旋转或平移操作，旋转后的图像保持原始图像的宽度和高度。
- angle：表示旋转的角度。注意，这里以角度为单位，而在三角函数中参数通常以弧度为单位；
- x_shift/y_shift：水平方向和垂直方向图像平移的大小，以像素为单位；
- x_origin/y_origin：旋转中心。旋转时图像绕着旋转中心以 CCW 方向（逆时针）旋转，旋转中心的表示也以像素为单位；
- use_mask：当 use_mask 设置为 TRUE，图像外的部分设置为黑色，否则按照镜像技术来设置图像外部分的颜色。

当同时设置了旋转和平移时，实验表明图像是先执行旋转操作后再平移。
> 函数 FreeImage_Rotate 和 FreeImage_RotateEx 类似，也执行旋转操作。不同的是该函数旋转后的图像大小和原始图像大小并不一致，而是保持一定的长宽比，通常会变得更大。因此，函数 FreeImage_Rotate 更适用于对图像旋转 90 度、180度和 270 度时的场景。

### 5.转换类型
```c
bitmap = FreeImage_ConvertToType(bitmap_temp, FIT_BITMAP);
```
将图像从一种类型转换为另一种类型。这里将 32 位字节流转换为位图格式。`FREE_IMAGE_TYPE` 用来指定 `FIBITMAP` 的位图类型（数据类型），可以是下面的常量值：
FIF           | Description
--------------|------------
FIF_UNKNOWN   |Unknown format (returned value only, never use it as input value)
FIT_BITMAP    |Standard image: 1-, 4-, 8-, 16-, 24-, 32-bit
FIT_UINT16    |Array of unsigned short: unsigned 16-bit
FIT_INT16     |Array of short: signed 16-bit
FIT_UINT32    |Array of unsigned long: unsigned 32-bit
FIT_INT32     |Array of long: signed 32-bit
FIT_FLOAT     |Array of float: 32-bit IEEE floating point
FIT_DOUBLE    |Array of double: 64-bit IEEE floating point
FIT_COMPLEX   |Array of FICOMPLEX: 2 x 64-bit IEEE floating point
FIT_RGB16     |48-bit RGB image: 3 x unsigned 16-bit
FIT_RGBA16    |64-bit RGBA image: 4 x unsigned 16-bit
FIT_RGBF      |96-bit RGB float image: 3 x 32-bit IEEE floating point
FIT_RGBAF     |28-bit RGBA float image: 4 x 32-bit IEEE floating point

当需要获取位图的数据类型时，可使用 `FreeImage_GetImageType` 来获取位图的数据类型。

### 6.保存文件
```c
FreeImage_Save(format, bitmap, "lenna_target.png");
```
将位图对象 `FIBITMAP` 保存到指定的文件中。第一个参数表示位图对应的格式。例如，当传递的参数是 `FIF_BMP` 时，则保存为 bmp 文件格式；第二个指向将要保存的位图对象；最后一个参数指定存储的文件名。

### 7.释放内存
```c
FreeImage_Unload(bitmap)
```
从内存中删除前面加载的 `FIBITMAP` 对象。一个位图对象是对地址空间中某个内存区域的引用。











