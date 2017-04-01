## 概述
在 OpenCL 中多个工作项会并行执行，当一个工作项对某个存储空间写入数据后，其它的工作项从该地址读取数据时，该如何保证后面工作项读取的数据是前一个工作项写入的数据。这时，就涉及到了工作项之间的同步。

## 工作项同步
OpenCL 中没有定义工作组之间工作项的同步操作，同步点（barrier）也无法对工作组中的部分工作项起作用，只能对工作组中所有的工作项同时生效。一个工作项对某个存储空间执行写操作，另一个工作项对该存储空间执行读操作，读到的数据不一定是前一个工作项写入的数据。使用存储次序同步操作（fence）可以保证前一个工作项的写对其它的工作项都可见。工作组中的工作项在执行内核代码时，执行到 barrier 会停止，直到所有的工作项都执行完 barrier 后才会一起继续往下执行。下面对 OpenCL 中工作项之间的同步函数进行描述，
```c
void barrier(cl_mem_fence_flags flags)
```
参数 `flags` 指定内存地址空间，可以是下面值的组合：

- CLK_LOCAL_MEM_FENCE：函数 barrier 将对存储在局部内存区域的变量执行 flush 操作，或者执行一个内存 fence 操作来保证对局部内存访问的正确操作顺序；
- CLK_GLOBAL_MEM_FENCE：函数 barrier 执行一个内存 fence 操作来保证对全局内存区域的正确访问顺序。在工作项对缓冲区或图像对象执行写操作，并需要读取更新后的数据时非常有用。

barrier 函数包含了一个内存 fence（读和写）操作，来保证对全局或局部内存区域执行正确的读写顺序。有下面两点需要注意：

- 如果 barrier 在条件语句中，如果有某个工作项会进入条件语句并执行这个 barrier 函数，那么所有的工作项必须进入条件语句；
- 如果 barrier 在一个循环语句中，所有的工作项在每次循环迭代时（barrier 后面的代码继续执行之前）必须执行这个函数。

## 注意
针对本文描述的示例，OpenCL 编程过程中有一些需要注意的地方。

### 变长数组
OpenCL C 中不支持像 C99 那样定义可变长数组。在 OpenCL 中，如果数组的长度不确定，设备端在编译的时候会报错。如下所示：
```c
__kernel void kernel_dot(__global int *dst, __global int *src1,  __global int *src2)
{
        int index = get_global_id(0);
        // size 值通过函数调用动态获取，在 OpenCL C 中会编译出错
        int size = get_global_size(0);   // ①
        //__local int buffer[WORKGROUP_SIZE];
        __local int buffer[size];

        // 缓冲区填充完成
        buffer[index] = src1[index] * src2[index];

        // 所有的工作项执行到这里。等待对局部缓冲区的访问完成
        barrier(CLK_LOCAL_MEM_FENCE);

        // 只有在第一个工作项执行的时候获取结果
        if (index == 0) {
                int sum = 0;
                //for (int i = 0; i < WORKGROUP_SIZE; i++) {
                for (int i = 0; i < size; i++) {
                        sum += buffer[i];
                }
                dst[0] = sum;
        }
}
```
上面内核代码中，在 ① 位置 `size` 的值通过调用内置函数获取，在函数返回之前它的值不确定。设备在编译该代码时，会出现如下错误：
```bash
1|shell@HWFRD:/data/local/tmp/sync/item $ ./opencl_sync_item
build log:
<source>:7:20: error: variable length arrays are not supported in OpenCL
        __local int buffer[size];
                          ^

error: Compiler frontend failed (error code 59)
```

### 跨工作组
下面的内核代码中，位置 ① 使用 __local 在局部内存区域中分配内存，分配的数据只能在同一工作组之间共享。假设工作组大小设置为 256（该值通过 CL_DEVICE_MAX_WORK_GROUP_SIZE 获取），全局工作项大小为 512（调用 clEnqueueNDRangeKernel 函数时，通过参数传入），就会对应两个工作组，此时 buffer 跨工作组不能共享。内核执行完后返回的数据只包含了前 256 个工作项的运算结果。
```c
__kernel void kernel_dot(__global int *dst, __global int *src1,  __global int *src2)
{
	int index = get_global_id(0);
	__local int buffer[WORKGROUP_SIZE];  // ①

	// 缓冲区填充完成
	buffer[index] = src1[index] * src2[index];

	// 所有的工作项执行到这里。等待对局部缓冲区的访问完成
	barrier(CLK_LOCAL_MEM_FENCE);

	// 只有在第一个工作项执行的时候获取结果
	if (index == 0) {
		int sum = 0;
		for (int i = 0; i < WORKGROUP_SIZE; i++) {
			sum += buffer[i];
		}
		dst[0] = sum;
	}
}
```

如果将 `__local int buffer[WORKGROUP_SIZE]` 重新声明为 `__global int buffer[WORKGROUP_SIZE]`，从全局内存区域中为变量分配内存会出错。构建时，就会出现如下错误，变量不能在全局地址空间声明错误。全局地址空间中分配的内存对应内存对象，包括缓冲区对象、图像对象等。
```bash
build log:
<source>:5:15: error: variable cannot be declared in global address space
        __global int buffer[WORKGROUP_SIZE];
                     ^

error: Compiler frontend failed (error code 59)
```








