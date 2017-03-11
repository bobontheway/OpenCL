

对于 async_work_group_copy 函数，工作组中每个工作项都要执行，而且传递的参数
需要保持一样，否则会出现预料之外的错误，例如：

// 执行异步拷贝操作
	event_t event = async_work_group_copy(&buffer[local_index],
		&src1[global_index], 1, 0);

不同的工作项，参数并不一样。

内核中的局部内存区域分配的内存，只分配一次吧？在工作组之间共享

问题：
只有第一个工作组中的工作项执行了局部内存区域中元素的算术运算。Why？

async_work_group_strided_copy
- 取元素或存放元素时，元素之间的间隔（间隔以元素个数为单位）。只作用于全局内存区域
