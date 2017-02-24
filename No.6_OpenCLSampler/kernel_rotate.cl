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
	int hWidth = width / 2;
	int hHeight = height / 2;
	int xt = x - hWidth;
	int yt = y = hHeight;

	// 计算旋转后坐标
	float2 readCoord;
	readCoord.x = (cosma * xt - sinma * yt) + hWidth;
	readCoord.y = (sinma * xt + cosma * yt) + hHeight;

	// 根据旋转后坐标读取元素图像元素值
	float4 value = read_imagef(srcImg, sampler, readCoord);
	write_imagef(dstImg, (int2)(x, y), value);
}
