__kernel void rotate_y(__global uchar *src, __global uchar *dst, int w, int h)
{
	int g_id_0 = get_global_id(0);
	int g_id_1 = get_global_id(1);

	dst[g_id_0*h + (h-1)-g_id_1] = src[w*g_id_1 + g_id_0];
}									     
