__kernel void kernel_dot(__global int *dst, __global int *src1,  __global int *src2)
{
	int item_offset = get_group_id(0) * WORKITEM_SIZE;

	__local int buffer[WORKITEM_SIZE];

	dst[get_global_id(0)] = src1[item_offset + get_local_id(0)];
#if 0
	// 执行异步拷贝操作
	event_t event = async_work_group_copy(buffer, &src1[item_offset],
		WORKITEM_SIZE, 0);

	// 等待异步拷贝完成
	wait_group_events(1, &event);

	buffer[get_local_id(0)] += 2;

	// 工作组中所有的工作项执行到这里。等待对局部缓冲区的访问完成
	barrier(CLK_LOCAL_MEM_FENCE);

	event = async_work_group_copy(&dst[item_offset], buffer,
		WORKITEM_SIZE, 0);

	// 等待异步拷贝完成
	wait_group_events(1, &event);
#endif

#if 0
	__local int buffer[WORKITEM_SIZE];
	int global_index = get_global_id(0);

	// 获取工作组中的每个工作项
	int local_index = get_local_id(0);

	// 执行异步拷贝操作
	event_t event = async_work_group_copy(&buffer[local_index],
		&src1[global_index], 1, 0);

	// 等待异步拷贝完成
	wait_group_events(1, &event);
	buffer[local_index] *= 2;

	// 工作组中所有的工作项执行到这里。等待对局部缓冲区的访问完成
	barrier(CLK_LOCAL_MEM_FENCE);

	event = async_work_group_copy(&dst[global_index], &buffer[local_index], 1, 0);

	// 等待异步拷贝完成
	wait_group_events(1, &event);
#endif
}
