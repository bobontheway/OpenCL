__kernel void tolower(__global char *in, __global char *out)
{
	int g_id = get_global_id(0);

	if ((in[g_id] >= 'A') && (in[g_id] <= 'Z'))
		out[g_id] = in[g_id] + 32;
	else
		out[g_id] = in[g_id];
}
