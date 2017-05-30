__kernel void memory_copy_v1(__global int *out, __global int *in)
{
	int g_id = get_global_id(0);
	out[g_id] = in[g_id];
}

__kernel void memory_copy_v2(__global int2 *out, __global int2 *in)
{
	int g_id = get_global_id(0);
	out[g_id] = in[g_id];
}

__kernel void memory_copy_v4(__global int4 *out, __global int4 *in)
{
	int g_id = get_global_id(0);
	out[g_id] = in[g_id];
}

__kernel void memory_copy_v8(__global int8 *out, __global int8 *in)
{
	int g_id = get_global_id(0);
	out[g_id] = in[g_id];
}

__kernel void memory_copy_v16(__global int16 *out, __global int16 *in)
{
	int g_id = get_global_id(0);
	out[g_id] = in[g_id];
}
