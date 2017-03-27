## No.7_2 事件回调函数

在调用 `clEnqueueXXXX` 系列函数时，会传递一个指向 cl_event 对象的指针。该指针接收事件对象，用来标识执行的命令。可以使用该事件对象来查询命令的执行状态，例如命令是否已经执行完成；也可以用来在其它的命令执行前等待事件对象对应的命令执行完成。

下面的 clSetEventCallback 函数对给定命令的执行状态注册一个回调函数。当命令执行状态和函数中的 `command_exec_callback_type` 参数一致时，会调用回调函数。命令的执行状态参见下表`命令状态及描述`：
```c
cl_int clSetEventCallback(
	cl_event event,
	cl_int  command_exec_callback_type,
	void (CL_CALLBACK  *pfn_event_notify)
		(cl_event event, cl_int event_command_exec_status, void *user_data),
	void *user_data)
```
参数说明如下所示：

- event：事件对象，由 `clEnqueueXXXX` 系列函数返回；
- command_exec_callback_type：命令的执行状态，当事件对象对应的命令执行状态是 command_exec_callback_type 指定的值时，则调用回调函数。可以给回调函数注册的命令执行状态值是 CL_COMPLETE，如果是其它的值，不能保证注册的回调函数在命令状态改变时，按照正确的顺序执行。当命令异常终止或成功完成执行后，将调用使用 CL_COMPLETE 来注册回调函数；
- pfn_event_notify：应用程序注册的事件对象回调函数。在 OpenCL 实现时，这个回调函数可能会以异步的方式调用。由应用程序来保证线程的安全性。回调函数的参数包括：
event：和回调函数关联的事件对象；
event_command_exec_status：该回调函数关联的命令的执行状态。如果回调函数是由于命令执行时异常终止，event_command_exec_status 将返回对应的错误值；
user_data：指针，指向用户提供的数据；
- user_data：当回调函数 pfn_event_notify 调用的时候，将作为的它 user_data 参数参数。

**命令状态及描述**

状态             | 描述
----------------|------------
CL_QUEUED       |命令已经提交到命令队列
CL_SUBMITTED    |提交到命令队列中的命令已被主机发送到设备端
CL_RUNNING      |设备当前正在执行该命令
CL_COMPLETE     |命令已执行完成

## 示例程序
该示例程序通过调用 clSetEventCallback 在指定的事件对象上注册一个回调函数，当命令的执行状态变为 `CL_COMPLETE` 时则调用该回调函数。为了进一步确认回调函数是否在命令执行状态为 CL_COMPLETE 调用，在回调函数中再通过指定事件对象调用 clGetEventInfo 来获取命令的执行状态。完整代码参见 xxx。
