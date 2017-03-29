工作组内工作项的同步；
OpenCL 原子操作；
工作组之间的同步；
=====================

OpenCL 没有定义工作组之间工作项的同步 API

同步点 barrier（work_group_barrier，OpenCL2.0）无法对工作组中指定的某
些工作项起作用，只能同时对工作组中所有的工作项起作用；

工作项同步
一个工作项对一个存储空间执行写操作，另一个工作项对该存储空间执行读操作，读到的数据
不一定是前一个工作项写入的数据。使用存储次序同步操作可以保证某一个工作项的写对其它
的工作项都可见

[barrier]
工作组中所有的工作项，在处理器上执行内核时，在允许 barrier
后面的代码继续执行之前，必须执行这个函数。
工作组中所有工作项在执行内核时，必须执行该函数。

如果 barrier 在条件语句中，如果某些工作项会进入条件语句并执行这个 barrier 函数，
那么所有的工作项必须进入条件语句。

如果 barrier 在一个循环语句中，所有的工作项在每次循环迭代时（在允许 barrier
后面的代码继续执行之前）必须执行这个函数。

barrier 函数也放了一个内存 fence（读和写），来保证对全局或局部内存正确的读写顺序。

flags 参数指定内存地址空间，可以设置为下面值的组合。

CLK_LOCAL_MEM_FENCE - barrier 函数 flush 一些存放在局部内存中的变量，
或者 queue 一个内存 fence 来保证对局部内存的正确内存操作顺序。

CLK_GLOBAL_MEM_FENCE - barrier 函数将存放一个内存 fence 来保证对全局内存正确的内
存操作次序。

这对工作项来说很有用，例如当工作项写到缓冲区或图像对象时，接着想读取更新后的数据。
（全局工作项之间的同步）

内存对象是对全局内存区域的引用。

在一些随后的 执行之前
必须执行这个函数

工作组中的所有工作项 必须执行

//======================
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
        // size 值通过函数调用动态获取，是否会发生错误？
        int size = get_global_size(0);
        //__local int buffer[WORKITEM_SIZE];
        __local int buffer[size];

        // 缓冲区填充完成
        buffer[index] = src1[index] * src2[index];

        // 所有的工作项执行到这里。等待对局部缓冲区的访问完成
        barrier(CLK_LOCAL_MEM_FENCE);

        // 只有在第一个工作项执行的时候获取结果
        if (index == 0) {
                int sum = 0;
                //for (int i = 0; i < WORKITEM_SIZE; i++) {
                for (int i = 0; i < size; i++) {
                        sum += buffer[i];
                }
                dst[0] = sum;
        }
}

===

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
        __global int buffer[WORKITEM_SIZE];
                     ^

error: Compiler frontend failed (error code 59)

==========

及，只能在 256 个工作项之间同步（单个工作组），如果需要在多个工作组之间同步怎么办？



