## 原子操作

## 概述
当多个线程在不同处理器上执行，同时访问相同内存时，会存在竞态条件，使用原子操作可以避免竞争。原子操作往往会对总线做一次锁步操作（lock-step），让当前总线上的访存操作能按照次序执行。同时又会刷新Cache，使得任一线程对全局变量使用了原子操作之后，其它所有线程都可见。

使用原子操作做同步对于执行而言开销是相当大的，但是想对于需要使用更原始的阻塞当前线程执行的同步方式而言又是比较高效的。因此，当对某些特定数据做同步更新时，不需要使用栅栏（fence）等这种更低效的同步处理机制，我们可以直接对那些存储地址采用原子操作。

## OpenCL 原子操作
OpenCL 提供了下面一系列原子操作。

### 原子加/减法
```c
int atomic_add (volatile __global int *p, int val)
```
在一个原子事务中执行。读取 p 指向位置的内容（用作返回值），将 p 指向位置的内容加上 val 后再存入该位置。
> 内核在多个设备上对同一内存位置执行的原子操作没法保证原子性。

原子减法操作 `atomic_sub` 和加法操作类似。

### 原子交换
```c
int atomic_xchg (volatile __global int *p, int val)
```
原子交换操作常用于对某个变量进行初始化或是作为互斥体（mutex）使用。将 val 存入 p 指向的位置，返回 p 位置修改之前的内容。

### 原子自增/自减
```c
int atomic_inc(volatile __global int *p)
```
原子加 1 操作。读取 p 指向位置的内容（用作返回值），将 p 指向位置的内容加上常量值 `1` 后再存入该位置。原子减 1 操作 `atomic_dec` 和加 1 操作类似。

### 原子比较交换
```c
int atomic_cmpxchg(volatile __global int *p,
	int cmp, int val)
```
如果 p 指向位置的内容和 cmp 相等，则将 val 存放 p 指向的位置，否则 p 指向位置的内容不变。返回 p 位置修改前的内容。


### 原子最小/最大值
```c
int atomic_min (volatile __local int *p, int val)
```
以原子的方式求最小值。读取 p 指向位置的内容（用作返回值），并将该值和 `val` 比较，然后在 p 指向的位置存入较小的值。求最大值 `atomic_max` 操作和求最小值操作类似。

### 原子与/或/异或
```c
int atomic_and (volatile __global int *p, int val)
```
以原子的方式执行按位与运算操作。读取 p 指向位置的内容（用作返回值），并将读取的值和 `val` 执行与操作，然后将将结果存入 p 指向的位置。原子按位或操作 `atomic_or` 和原子按位异或 `atomic_xor` 操作与原子按位与操作类似。

## 示例程序
该示例程序分别对每个工作组中工作项做点乘运算，然后将它们以原子的方式相加到全局内存变量 `dst` 中，供主机端获取。
```
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
```

## 参考

- OpenCL异构并行计算:原理、机制与优化实践