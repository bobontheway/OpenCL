## 概述
在 OpenCL 中多个工作项会并行执行，当一个工作项对某个存储空间写入的数据后，其它的工作项会从该地址读取数据时，
该如何保证后面工作项读取的数据是前一个工作项写入的数据。这时，就涉及到了工作项之间的同步。

## OpenCL 工作项之间同步
OpenCL 中没有定义工作组之间工作项的同步操作，同步点（barrier）也无法对工作组中的部分工作项起作用，只能对工作组中所有的工作项同时生效。
一个工作项对某个存储空间执行写操作，另一个工作项对该存储空间执行读操作，读到的数据不一定是前一个工作项写入的数据。使用存储次序同步操
作（fence）可以保证前一个工作项的写对其它的工作项都可见。工作组中的工作项在执行内核代码时，执行到 barrier 会停止，直到所有的工作项都执行完 barrier 后才会一起继续往下执行。

下面对 OpenCL 中工作项之间的同步函数进行描述：
void barrier(cl_mem_fence_flags flags)

参数 `flags` 指定内存地址空间，可以是下面值的组合：
- CLK_LOCAL_MEM_FENCE：函数 barrier 将对存储在局部内存区域的变量执行 flush 操作，或者
执行一个内存 fence 操作来保证对局部内存访问的正确顺序；
- CLK_GLOBAL_MEM_FENCE：函数 barrier 执行一个内存 fence 操作来保证对全局内存区域的正确访问顺序。
在工作项对缓冲区或图像对象执行写操作，并需要读取更新后的数据时非常有用。

barrier 函数包含了一个内存 fence（读和写）操作，来保证对全局或局部内存区域执行正确的读写顺序。下面两点需要注意：

- 如果 barrier 在条件语句中，如果有某个工作项会进入条件语句并执行这个 barrier 函数，那么所有的工作项必须进入条件语句。
- 如果 barrier 在一个循环语句中，所有的工作项在每次循环迭代时（barrier 后面的代码继续执行之前）必须执行这个函数。


内存对象是对全局内存区域的引用。

注意，示例：

示例一

在 OpenCL 中，如果数组的长度不确定，编译错误（设备端）。
P255 中，OpenCL C 中不支持像 C99 那样定义可变长数组。
======================

1|shell@HWFRD:/data/local/tmp/sync/item $ ./opencl_sync_item
build log:
<source>:7:20: error: variable length arrays are not supported in OpenCL
        __local int buffer[size];
                          ^

error: Compiler frontend failed (error code 59)

//== 数组

__kernel void kernel_dot(__global int *dst, __global int *src1,  __global int *src2)
{
        int index = get_global_id(0);
        // size 值通过函数调用动态获取，在 OpenCL C 中会编译出错
        int size = get_global_size(0);
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

示例二：

最大工作项为 256，max_work_group_size = 256。由于内核代码中
使用 __local int buffer[size]; 由于 __local
变量定义局部内存（在同一工作组之间共享），当工作项定义为 512
时，会使用两个工作组（跨工作组），此时 buffer 跨工作组不能共享。
及结果只包含前 256 个工作项的数据。

如果将 "__local int buffer[size]" 定义为 "__global int buffer[size]"
会发生错误。构建时，说变量不能在全局地址空间声明。
也就是说全局地址空间的都是内存对象。

==========
/opencl_sync_item                                                             <
build log:
<source>:5:15: error: variable cannot be declared in global address space
        __global int buffer[WORKGROUP_SIZE];
                     ^

error: Compiler frontend failed (error code 59)

==========

及，只能在 256 个工作项之间同步（单个工作组），如果需要在多个工作组之间同步怎么办？





















## 实现
