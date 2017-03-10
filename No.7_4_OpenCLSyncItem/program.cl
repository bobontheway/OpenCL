__kernel void kernel_dot(__global int *dst, __global int *src1,  __global int *src2)
{
	int index = get_global_id(0);
	__local int buffer[WORKITEM_SIZE];

	// 缓冲区填充完成
	buffer[index] = src1[index] * src2[index];

	// 所有的工作项执行到这里。等待对局部缓冲区的访问完成
	barrier(CLK_LOCAL_MEM_FENCE);

	// 只有在第一个工作项执行的时候获取结果
	if (index == 0) {
		int sum = 0;
		for (int i = 0; i < WORKITEM_SIZE; i++) {
			sum += buffer[i];
		}
		dst[0] = sum;
	}
}
