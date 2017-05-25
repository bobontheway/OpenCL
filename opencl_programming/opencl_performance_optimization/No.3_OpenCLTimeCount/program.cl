#define INSIZE	8388608 /* 8MB */

__kernel void memory_read_v2(__global int *in, __global int *out)
{
	int g_id = get_global_id(0);

	if ((in[g_id] >= 'A') && (in[g_id] <= 'Z'))
		out[g_id] = in[g_id] + 32;
	else
		out[g_id] = in[g_id];
}
