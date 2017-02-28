使用 OpenCL 自带的采样器来旋转图像。

[约定]
传递给 read_imagef/write_imagef 函数 x 的坐标范围是 [0, width-1]，同时传递
给 read_imagef/write_imagef 函数 y 的坐标范围是 [0, height-1];

使用图像的宽度和高度作为二维全局工作项的大小，每个维度的全局工作项标识就可以
作为采样坐标。

[采样器的作用？]
采样器只有读操作还是包含读写操作？

先把坐标 A'(x, y) 转换为相对于中心坐标的点，旋转后再还原为正常坐标。

实际上是从 A' 点取值，然后放回 A 点，即，顺时针旋转了 beita 的角度

顺时针和逆时针的区别：
如果从 A 取值，写到 A'，为逆时针旋转；
如果是从 A' 采样，写到 A，则是顺时针旋转，可以对这两个点的坐标值分别采样。

=======================================================================

[0.图像内存对象和普通内存对象区别]
内存对象是对全局内存区域的引用，图像内存对象表示其所引用的全局内存区域所存储的数据按指定的图像格式存放。


[1.用在哪里]
-图像读写函数只能用于图像内存对象。
-通过函数调用，访问内存对象，可以对图像指定位置执行读、写或查询操作。


[2.怎么用]
-内核执行`读`访问的图像对象在声明的时候需要使用 __read_only 限定符修饰；
-内核执行`写`操作的图像对象在声明的时候需要使用 __write_only 限定符修饰。
-同一个图像内存对象不能同时支持读写操作，限定符使用不当将引发编译错误。


[3.坐标怎么决定]


[4.采样器声明]


[5.图像通道和颜色值]
image channels 和  color values


2.有什么作用
 
read_image 返回由四个通道组成的颜色值，使用 x、 y、 z 和 w 来表示。x 表示红色，y 表示绿色，z 表示蓝色，w 表示 alpha 通道。

3.好处


float4 read_imagef(image1d_t  image,
 	sampler_t  sampler,
 	int  coord)

采样器对象描述了当在内核中读取图像数据时如何对图像采样，图像读取函数 read_imagef 包含一个采样器参数。
该参数可以是通过 OpenCL 函数创建采样器对象，然后使用 clSetKernelArg 传递给内核；也可以在内核函数中
声明，在内核程序中的采样器对象需要声明为 sampler_t 类型常量。

采样器对象包含了一些属性，这些属性控制如何读取图像对象的元素。分别是规格化浮点坐标，寻址模式和过滤模式。

- 规格化坐标，指定传递的 x、y 和 z 坐标值是以规格化浮点坐标或者非规格化坐标值。必须是下面的枚举类型的值
CLK_NORMALIZED_COORDS_TRUE 或者 CLK_NORMALIZED_COORDS_FALSE。

- 寻址模式，指定图像的寻址模式，当超过图像坐标区域是如何处理。必须是下面的枚举类型的值：

CLK_ADDRESS_MIRRORED_REPEAT，图像区域外的坐标设置为区域内坐标的反射值对应的颜色；
CLK_ADDRESS_REPEAT， 图像区域外的坐标重复区域内坐标的颜色，只对规格化坐标有效；
CLK_ADDRESS_CLAMP_TO_EDGE，图像区域外的坐标返回图像边缘的颜色；
CLK_ADDRESS_CLAMP，图像区域外的坐标返回边框的颜色；

4.边框颜色定义
如果采样器的寻址模式为 CLK_ADDRESS_CLAMP，超过图像区域的坐标返回边框的颜色。
边框颜色选择依赖图像的颜色通道，它可以是下面的颜色值：
如果图像颜色通道是 CL_A, CL_INTENSITY, CL_Rx, CL_RA, CL_RGx, CL_RGBx, CL_ARGB, CL_BGRA, or CL_RGBA，
边框颜色为 (0.0f, 0.0f, 0.0f, 0.0f)
如果图像颜色的通道是 CL_R, CL_RG, CL_RGB 或 CL_LUMINANCE，边框颜色为 (0.0f, 0.0f, 0.0f, 1.0f)。

- 过滤模式
指定使用的过滤模式。可以是 CLK_FILTER_NEAREST 或 CLK_FILTER_LINEAR 枚举值，分别表示最近邻插值和双线性插值。


公式：
（图像旋转公式）

`弧度`和`度`是度量角大小的两种不同单位，在三角函数里`角`以`弧度`为单位。
弧度表示当弧长正好等于圆的半径时，夹角的大小为 1 弧度，360 度角对应的弧长为 2pai r，所以弧度为 2pai,
因此，180 度角对应的弧度为 pai。



传递给 read_imagef/write_imagef 函数 x 的坐标范围是 [0, width-1]，同时传递
给 read_imagef/write_imagef 函数 y 的坐标范围是 [0, height-1];

先把坐标 (x, y) 转换为想对于中心坐标的点，旋转后再还原为正常坐标。

实际上是从 B 点取值，然后放回 A 点。

radians






[参考]
https://anteru.net/blog/2014/04/30/2396/index.html
Live OpenCL application profiling with AMD's GPUPerfAPI

弧度和度怎么区别
http://blog.csdn.net/hongxiali/article/details/5355954


http://baike.baidu.com/item/%E5%BC%A7%E9%95%BF%E8%AE%A1%E7%AE%97%E5%85%AC%E5%BC%8F

AMD OpenCL 大学课程
http://max.book118.com/html/2016/0103/32612630.shtm


Markdown 中插入数学函数
http://blog.csdn.net/xiahouzuoxin/article/details/26478179




