// 一个计算设备上，可以同时并发执行的工作组个数是有限的。
// 这意味着，如果当前所有活跃的工作组对某个或某些非活跃的工作组产生依赖等待，
// 那么内核程序会被挂死。因为一个活跃的工作组需要被隐退（也就完全执行完之后）
// 才能把非活跃的工作组调度进来执行。

// 补充：这个的活跃应该指的是正在执行中……
// 隐退 - 指的是执行完成之后（例如线程执行完后，切换到新的线程执行）

// 即，这个程序就是测试内核函数是否会返回，正常情况下是会返回的。
__kernel void kernel_test(__global int *dst, __global int *src1,  __global int *locks)
{
	// 工作组索引，其值和工作组的个数有关
	const int gid = get_group_id(0);

	// 局部工作项索引，其值和局部工作项个数有关
	const int index = get_local_id(0);

	// 如果是工作组中的第一个工作项，则进入分支
	if (index == 0) {
		// 工作组索引号是计算单元数目的 1.5 倍，
		// 某些工作组才会执行下面的分支操作。
		if (gid < MAX_COMPUTE_UNITS * 3 / 2)
			while (dst[1] == 0)
				;
		// 这个工作组可以让其它的工作组推出 while 循环
		if (gid = 10000)
			dst[1] = 1;

		// 其它的工作组退出 while 循环后以原子的方式对变量 dst
		// 执行加法操作
		atomic_add(dst, 1);
	}
}