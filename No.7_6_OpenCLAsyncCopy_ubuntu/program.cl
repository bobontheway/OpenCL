__kernel void kernel_dot(__global int *dst, __global int *src1,  __global int *src2)
{
	// 定义局部缓冲区，在同一工作组的工作项之间共享
	local int buffer[WORKITEM_SIZE];

	// 每个工作组中第一个工作项的偏移
	const size_t offset = get_group_id(0) * WORKITEM_SIZE;

	// 执行异步拷贝操作
	event_t event = async_work_group_copy(buffer, &src1[offset], WORKITEM_SIZE, 0);
	
	// 获取工作组中的每个工作项
	const int index = get_local_id(0);

	// 等待异步拷贝完成
	wait_group_events(1, &event);

	buffer[index] *= 2;

	// 工作组中所有的工作项执行到这里。等待对局部缓冲区的访问完成
	barrier(CLK_LOCAL_MEM_FENCE);

	event = async_work_group_copy(&dst[offset], buffer, WORKITEM_SIZE, 0);

	// 等待异步拷贝完成
	wait_group_events(1, &event);
#endif
}
