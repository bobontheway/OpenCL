

内核中的局部内存区域分配的内存，只分配一次吧？在工作组之间共享

问题：
只有第一个工作组中的工作项执行了局部内存区域中元素的算术运算。Why？

async_work_group_strided_copy
- 取元素或存放元素时，元素之间的间隔（间隔以元素个数为单位）。只作用于全局内存区域