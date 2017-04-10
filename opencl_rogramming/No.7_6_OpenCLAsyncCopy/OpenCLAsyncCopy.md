## 简介
OpenCL C 编程语言内置了类似 `memcpy` 的函数，内核代码通过调用该函数，可以在局部内存和全局内存区域之间拷贝数据。当设备内存独立于主机内存时，无需主机内存参与数据周转，就能在不同内存区域之间拷贝数据，这提高了数据拷贝效率。

## 函数描述

### 异步拷贝
下面的 OpenCL C 内置函数在全局内存和局部内存区域之间执行异步拷贝，同时还从全局内存区域预取数据到全局高速缓存。
```c
event_t async_work_group_copy(__local gentype *dst,
	const __global gentype *src,
	size_t  num_gentypes,
	event_t event)

event_t async_work_group_copy(__global gentype *dst,
	const __local gentype *src,
	size_t num_gentypes,
	event_t event)
```
以异步方式，从内存区域 `src` 拷贝 `num_gentypes` 个元素到内存区域 `dst`。工作组中所有工作项都会执行异步拷贝操作，也就是说该内置函数必须被工作组中的每个工作项执行，而且传递的参数需要保持相同，否则会出现不可预知的错误。例如，下面代码中：
```c
local_index = get_local_id(0);
event_t event = async_work_group_copy(&buffer[local_index],
			&src1[global_index], 1, 0);
```
对于不同的工作项，传递的参数并不一致，这时就会出现不可预知的错误。

这些异步拷贝函数返回一个 `event_t` 类型的事件对象，`wait_group_events` 可以通过它们来等待异步拷贝操作执行完成。如果有多个异步拷贝操作，可以将它们关联为同一个事件。将前面返回的事件对象 `event` 作为后面 `async_work_group_copy` 调用的参数即可，这就允许一个事件被多个异步拷贝共享。如果不需要共享，在调用 async_work_group_copy 函数时将 `event` 参数设置为 0 即可。如果 `event` 参数为非 0 值，将在调用 `async_work_group_copy`  后返回相同值。

> 注意：该函数并没有执行任何隐式的数据源同步，这就需要在执行拷贝之前使用一个 barrier 来做同步。

### 事件等待
等待事件，用来表示 async_work_group_copy 操作已经完成。在该等待事件函数执行返回后，event_list 中的事件对象将被释放。
```c
void wait_group_events(int num_events, event_t *event_list)
```
该函数必须被工作组中所有工作项执行，并且它们在执行内核时使用的 `num_events` 和 `event_list` 中指定的事件对象必须相同，否则会出现不可预知的结果。

> 注意：在退出之前，内核必须使用 `wait_group_events` 内置函数来等待所有的异步拷贝操作完成，否则会出现不可预知的结果。

### 预取操作
从全局内存区域预取数据到全局高速缓存。
```c
void prefetch(const __global gentype *p, size_t num_gentypes)
```
从全局内存区域预取 `num_gentypes` \* sizeof(gentype) 字节到全局高速缓存。该预取指令用于工作组中的某个工作项，而且并不会影响内核函数的功能。

## 示例程序
该示例程序在 Ubuntu 平台上执行，内核代码在局部内存区域声明了一个缓冲区 `buffer`，其长度为工作组大小。然后将全局内存中的数据拷贝到局部内存区域，待拷贝完成后每个工作项根据其局部 ID 标识更新缓冲区。最后以工作组大小为单位再将其拷贝回全局内存区域，内核代码如下：
```c
__kernel void kernel_dot(__global int *dst, __global int *src1,  __global int *src2)
{
	// 定义局部缓冲区，在同一工作组的工作项之间共享
	__local int buffer[WORKGROUP_SIZE];

	// 每个工作组中第一个工作项的偏移
	const size_t offset = get_group_id(0) * WORKGROUP_SIZE;

	// 执行异步拷贝操作
	event_t event = async_work_group_copy(buffer, &src1[offset], WORKGROUP_SIZE, 0);

	// 获取工作组中的每个工作项
	const int index = get_local_id(0);

	// 等待异步拷贝完成
	wait_group_events(1, &event);

	buffer[index] *= 2;

	// 工作组中所有的工作项执行到这里。等待对局部缓冲区的访问完成
	barrier(CLK_LOCAL_MEM_FENCE);

	event = async_work_group_copy(&dst[offset], buffer, WORKGROUP_SIZE, 0);

	// 等待异步拷贝完成
	wait_group_events(1, &event);
}
```

## 参考

- OpenCL异构并行计算:原理、机制与优化实践
- https://www.khronos.org/registry/OpenCL/sdk/1.1/docs/man/xhtml/
