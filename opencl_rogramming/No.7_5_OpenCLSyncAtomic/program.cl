__kernel void kernel_dot(__global int *dst, __global int *src1,  __global int *src2)
{
	int global_index = get_global_id(0);
	__local int buffer[WORKGROUP_SIZE];

	// 以原子方式将 dst 地址中内容清零（只执行一次，使用全局工作项标识）
	if (global_index == 0)
		atomic_xchg(dst, 0);

	// 获取工作组中的每个工作项
	int local_index = get_local_id(0);

	// 填充缓冲区
	buffer[local_index] = src1[global_index] * src2[global_index];

	// 所有的工作项执行到这里。等待对局部缓冲区的访问完成
	barrier(CLK_LOCAL_MEM_FENCE);

	// 只有在第一个工作项执行的时候获取结果，并将其添加到 dst 指向的位置
	if (local_index == 0) {
		int sum = 0;
		for (int i = 0; i < WORKGROUP_SIZE; i++)
			sum += buffer[i];
		atomic_add(dst, sum);
	}
}
