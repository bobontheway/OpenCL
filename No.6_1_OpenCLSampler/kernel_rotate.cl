__constant  sampler_t sampler = CLK_NORMALIZED_COORDS_FALSE |
	CLK_FILTER_NEAREST | CLK_ADDRESS_CLAMP;

__kernel void rotate_rgba(__read_only image2d_t srcImg,
	__write_only image2d_t dstImg)
{
	const int x = get_global_id(0);
	const int y = get_global_id(1);

	// 根据旋转后坐标读取元素图像元素值
	float4 value = read_imagef(srcImg, sampler, (int2)(x, y));
	write_imagef(dstImg, (int2)(x, y), value);
}
