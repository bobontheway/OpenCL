## 概述
MAC OS
CL_QUEUED 和 CL_SUBMITTED 在主机端维护，而 CL_RUNNING 和 CL_COMPLETE
在设备端维护。需要通过 clFinish 或 clWaitForEvents 将设备端的事件同步到主机端
（CL_XXXX 指状态，事件还是要用 event ?）

而在 Windows 或 Linux 下，如果使用 AMD 的 OpenCL 实现，在主机端可以捕获到
CL_COMPLETE 状态，而不能捕获 CL_RUNNING 状态

Android
在 ARM Mali 下，非阻塞的方式，一直处于 CL_ENQUEUED 状态；如果是以阻塞的方式，
首次调用时，直接显示 CL_COMPLETE 状态；（为什么非阻塞状态时间不变呢）

调用 clEnqueueWriteBuffer 所在的线程必须与调用 clGetEventInfo
在同一个线程，如果在不同的线程中可能引发异常；

使用异步方式来跟踪事件状态变化（状态每次变化后都会调用？），

通过事件对象做命令之间的同步

只等待主机端的某些命令（不是全部）完成后，才继续往下执行，可以使用
clWaitForEvents
函数。它会将主机端的命令挂起，直到事件列表（event_list）中的所有事件已经完成为止

标签用来标识某些对象（表示特定语义）



## 实现
