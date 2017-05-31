#define INSIZE	8388608 /* 8MB */

__kernel void memory_copy_v1(__global int *out, __global int *in)
{
	int g_id = get_global_id(0);
	out[g_id] = in[g_id];
}
