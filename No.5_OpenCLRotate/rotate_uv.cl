__kernel void rotate_uv(__global uchar *src, __global uchar *dst, int halfW, int halfH, int pBase)
{
	int g_id_0 = get_global_id(0);
	int g_id_1 = get_global_id(1);

	dst[pBase + (g_id_0 * halfH + (g_id_1-1))] = src[pBase + ((halfH-g_id_1) * halfW) + g_id_0];
}												    
