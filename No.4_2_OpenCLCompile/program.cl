#include "lower.h"
__kernel void toupper(__global char *in, __global char *out)
{
	int g_id = get_global_id(0);

	if ((in[g_id] >= 'A') && (in[g_id] <= 'Z'))
		out[g_id] = lower(in[g_id]);
	else
		out[g_id] = in[g_id];
}
