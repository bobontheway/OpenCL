## OpenCL 同步——事件同步

## 简介
clFinish 会进入阻塞状态，直到前面提交到命令队列的 OpenCL 命令发送到和命令队列关联的设备，并且执行完成。它作为一个同步点，会等待命令队列中所有的 OpenCL 命令执行完成，该操作对某些时候只需两个命令的同步或少数几个命令的同步来说开销较大。使用事件对象来同步就很好的解决了该问题，它用于某些指定事件的同步。

## 执行内核
在介绍具体示例之前，先通过 `clEnqueueNDRangeKernel` 函数来介绍事件对象。
```bash
cl_int clEnqueueNDRangeKernel(cl_command_queue command_queue,
 	cl_kernel kernel,
 	cl_uint work_dim,
 	const size_t *global_work_offset,
 	const size_t *global_work_size,
 	const size_t *local_work_size,
 	cl_uint num_events_in_wait_list,
 	const cl_event *event_wait_list,
 	cl_event *event)
```
相关参数描述如下：

- command_queue：命令队列。内核程序将被提交到这个命令队列，并且在和该命令队列关联的设备上执行；
- kernel：内核对象。`kernel` 和 `command_queue` 关联的 OpenCL 上下文必须相同；
- work_dim：维度数，每个维度有单独全局工作项和工作组中的工作项。`work_dim` 必须大于 0，并小于 CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS；
- global_work_offset：对应 ` work_dim` 的数组。用来描述每个维度中全局工作项开始的 ID 标识。如果 global_work_offset 为 NULL，全局 ID 标识对应的偏移为（0,0……0）；
- global_work_size：对应 ` work_dim` 的数组。用来描述每个维度中全局工作项的数目。总的全局工作项数目计算方式为， global_work_size[0] *...* global_work_size[work_dim-1]；
- local_work_size：对应 ` work_dim` 的数组。用来描述每个工作组中工作项的数目，又称作工作组的大小。一个工作组中工作项的总数计算方式为，local_work_size[0] \*...\* local_work_size[work_dim-1]。工作组中工作项的数目必须小于或等于 CL_DEVICE_MAX_WORK_GROUP_SIZE 的大小，并且 local_work_size[0]... local_work_size[work_dim-1] 中工作项的数目必须小于或等于 CL_DEVICE_MAX_WORK_ITEM_SIZES[0]... CL_DEVICE_MAX_WORK_ITEM_SIZES[work_dim-1] 中对应的值。local_work_size 的值可以为 NULL，在这种情况下，由 OpenCL 的实现来决定如何将工作项划分到不同的工作组；
- event_wait_list/num_events_in_wait_list：指定在这个命令执行之前需要完成的事件。如果 event_wait_list 为 NULL，那么该命令执行之前无需等待任何命令。如果 event_wait_list 为 NULL，num_events_in_wait_list 必须为 0。如果 event_wait_list 不为 NULL，event_wait_list 列表中指向的事件必须有效，并且 num_events_in_wait_list 必须大于 0。event_wait_list 列表中的事件作为一个同步点。`event_wait_list` 和 `command_queue` 必须关联同一个上下文。和 event_wait_list 关联的内存在函数返回后，可以重新使用或释放；
- event：返回一个事件对象，用来标识执行的内核实例。事件对象是唯一的，并且在随后可以用来标识一个特定的内核执行实例。如果 `event` 为 NULL，执行的内核实例不会创建对应的事件，这样应用程序将不能查询它的执行情况，也不能在命令队列中等待该实例执行完成。

在该函数中，输入的事件对象存放在 `event_wait_list` ，表示在该命令执行之前需要等待哪些命令执行完成。输出的事件对象 `event` 用来标识执行的命令，应用程序可以通过该事件对象查询命令的执行状态；其它命令在执行之前可以通过该事件对象等待命令执行完成。

## 事件对象的操作

### 增加事件引用计数
```bash
cl_int clRetainEvent(cl_event event)
```
参数 `event` 表示需要增加引用计数的事件对象，如果成功执行返回 CL_SUCCESS。

> 注意：OpenCL 命令在返回一个事件对象时，执行了隐式的引用计数增加操作。

### 减少事件引用计数
```bash
cl_int clReleaseEvent(cl_event event)
```
参数 `event` 表示需要释放引用计数的事件对象。一旦引用计数降低为 0，事件对象将删除。

## 示例程序
该示例程序创建了两个输入缓冲区对象，一个输出缓冲区对象。OpenCL 内核接收输入缓冲区对象中的数据，运算之后将结果存放到输出缓冲区，然后主机端读取结果到内存中。整个过程需要保证主机端在读取输出缓冲区中的内容时，内核已将完整的数据存放到输出缓冲区。本文使用事件对象在这两个操作之间执行同步。















