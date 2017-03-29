## OpenCL 主机端同步
主机端的同步主要是对命令队列的操作。在将一个 OpenCL 命令提交到命令队列的时候，有个参数用来标识是以阻塞还是非阻塞的方式执行。

- 阻塞：该函数会等待，直到命令执行完成后才返回。当后续处理需要依赖该函数处理的结果时，可在该函数返回后放心使用它返回的数据；
- 非阻塞：函数无需等到执行的任务完成就返回。该线程接下来可以马上处理其它的事情，以提高并行效率。如果后续处理依赖该函数返回的数据就要使用阻塞的方式。

除了在将 OpenCL 命令提交到命令队列时执行同步操作，也可以通过调用下面的命令执行显式的同步操作：

## clFlush 操作
将前面提交到命令队列中的 OpenCL 命令发送到和这个命令队列关联的设备。
```c
cl_int clFlush (cl_command_queue command_queue)
```
clFlush 只保证所有提交到 command_queue 命令队列中的命令发送到了对应的设备，并不能保证在 clFlush 返回后这些命令已经执行完成。

> 值得注意的是，某些以阻塞的方式提交到命令队列中的 OpenCL 命令执行了一个隐式的 flush 动作。这样的函数返回后，命令队列中的所有命令已经发送到了相应的设备。


## clFinish 操作
```c
cl_int clFinish (cl_command_queue command_queue)
```
该函数进入阻塞状态，直到前面提交到 `command_queue` 命令队列的 OpenCL 命令发送到对应的设备，并且已经执行完成。clFinish 函数是使用的比较频繁的同步点。

## 示例程序
该示例在主机端分配了一个内存和两个缓冲区对象，然后分别以阻塞的方式和非阻塞的方式将主机端内存中的内容写到缓冲区对象所引用的全局内存区域中。程序执行完成后，以阻塞方式和非阻塞方式来写缓冲区对象所使用的时间如下：
```bash
shell@HWFRD:/data/local/tmp/opencl/sync/host $ ./opencl_sync
Test: write memory object1, 47673.958000 us
Test: write memory object2, 31721.875000 us
Test: write memory object1 non-block, 39.063000 us
Test: write memory object2 non-block, 28.646000 us
Test: finish command queue, 20805.729000 us
```
可以发现，使用阻塞的方式将主机端内存中的数据写到缓冲区对象所花的时间分别是 47673 微妙和 31721 微秒；而非阻塞时，所用的时间非别是 39 微秒和 28 微秒。执行 clFinish，等待直到命令队列中的 OpenCL 命令执行完成，所花的时间为 20805 微秒。


## 参考

- OpenCL异构并行计算:原理、机制与优化实践
