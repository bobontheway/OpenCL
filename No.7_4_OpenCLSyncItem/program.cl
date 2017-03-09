__kernel void kernel_add(__global int *dst, __global int *src1,  __global int *src2)
{
	int index = get_global_id(0);
	dst[index] = src1[index] + src2[index];
}


__kernel void kernel_mul(__global int *dst, __global int *src1,  __global int *src2)
{
	int index = get_global_id(0);
	dst[index] = dst[index] * src1[index] - src2[index];
}
