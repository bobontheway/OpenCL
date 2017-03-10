async_work_group_copy/wait_group_event - 内置函数

在全局和局部内存区域执行异步拷贝，并从全局内存区域执行预取操作；
工作组中的所有工作项都会执行该异步拷贝操作，并使用相同的参数（否则结果不可预知）。

返回的事件对象可以被 wait_group_events 使用，用来等待异步拷贝执行完成。
event 参数可以用来将 async_work_group_copy 和前面的异步拷贝关联，这样允许一
个事件在多个异步拷贝之间共享；否则，event 传入 0 即可。

如果 event 参数是非零，event 中提供的事件对象将被返回；

在主机端的事件类型为 cl_event，而 OpenCL 内核程序中也含有事件对象
类型，定义为 event_t。事件对象可以用来跟踪异步拷贝的完成情况（异
步拷贝为内置函数）

gentype 是泛型类型，能支持 OpenCL 中的所有标量及向量形式的基本类型，
参数 num_gentypes 指明了需要拷贝多少个 gentype 的元素。因此，一共
需要拷贝的字节数为 num_gentypes * sizeof(gentype)


wait_group_event - 和上面的函数一样，必须要当前工作组中的所有工作项参数进来
操作。

