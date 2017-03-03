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

	// 相对于圆心的点，临时变量（将圆心看作坐标原点）
	int xt = x - ox;
	int yt = y - oy;

	// 计算旋转后坐标。y 是右上方上面的点（顺时针旋转）
	float2 readCoord;
	readCoord.x = (cosma * xt - sinma * yt) + ox;
	readCoord.y = (sinma * xt + cosma * yt) + oy;

	// 根据旋转后坐标读取元素图像元素值
	float4 value = read_imagef(srcImg, sampler, readCoord);
	write_imagef(dstImg, (int2)(x, y), value);
}
