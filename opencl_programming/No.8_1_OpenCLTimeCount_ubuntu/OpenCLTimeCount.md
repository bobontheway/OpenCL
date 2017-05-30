# OpenCL 性能 —— 时间测量

## 时钟和时间

在 `OpenCL 编程实践`中，为了比较 CPU 和 GPU 的性能，前面统计函数的执行时间使用了 `glibc` 中提供的时钟和时间相关函数。为了测得到 `func` 函数的执行时间，在待测函数执行前，首先通过调用 system_time() 得到当前的时间戳 `time_start`，接着执行 `func` 函数，待其执行完后再次调用 system_time() 得到当前时间戳 `time_end`，使用 time_end 减去 time_start 就可以得到函数 `func` 的执行时间。下面是封装后的 system_time() 函数：
```c
int64_t system_time()
{
	struct timespec t;

	clock_gettime(CLOCK_MONOTONIC, &t);
	return (int64_t)(t.tv_sec) * 1e9 + t.tv_nsec;
}
```

函数 clock_gettime() 的原型如下：
```c
int clock_gettime(clockid_t clk_id, struct timespec *tp);

struct timespec {
	time_t   tv_sec;        /* seconds */
	long     tv_nsec;       /* nanoseconds */
};
```

下面是参数描述：

- clk_id：表示时钟类型。CLOCK_MONOTONIC 为单调递增时钟，在系统启动后开始计时，其时间不能手动的被重新设置。在 Linux 内核中，CLOCK_MONOTONIC 通过高精度定时器实现，使用 hrtimer_init() 来初始化；
- tp：指向结构体 timespec 的指针，返回当前的时间戳信息。

在 Linux 系统中，内核版本从 `Linux 2.6.28 开始`，支持另一个时钟 CLOCK_BOOTTIME。该时钟和 CLOCK_MONOTONIC 比较类似，但是它包含了系统在休眠过程中的时间，这在记录的时间戳信息需要包含休眠时间时非常有用。相关时钟的精度可通过 clock_getres() 函数调用获取，其原型如下：
```c
int clock_getres(clockid_t clk_id, struct timespec *res);
```
下面是参数描述：

- clk_id：表示对应时钟的类型；
- res：返回该时钟的精度，以纳秒为单位。

在下面的示例代码中，为了更准确的计算函数 `clEnqueueNDRangeKernel` 的执行时间，在其前后分别调用了 clFinish() 函数。在 clEnqueueNDRangeKernel() 的前面调用 clFinish() 函数，可以避免在执行 clEnqueueNDRangeKernel 函数时，提交到命令队列中的其它命令对它产生影响。通过添加 clFinish() 函数能保证在执行 clEnqueueNDRangeKernel() 函数之前，提交到命令队列的其它命令已经执行完成；在 clEnqueueNDRangeKernel() 之后又再次调用了 clFinish() 函数，这由于 `clEnqueueNDRangeKernel` 是异步执行，在函数返回时，该函数并没有执行完成。通过添加 clFinish() 函数就可以让当前的 CPU 进入阻塞状态，直到命令队列中的命令执行完成之后才返回。
```c
clFinish(queue);
int64_t time_start = system_time();
err = clEnqueueNDRangeKernel(queue, kernel, 1,
	NULL, g_size, local_size,
	0, NULL, NULL);
clFinish(queue);
int64_t time_end = system_time();
printf("kernel execute time: %f(um)\n", (time_end-time_start)/1e3);
```

## Profiling 操作
所谓 Profiling 操作就是更准确的测量函数/内核的执行时间，以此对系统的性能进行分析。找到内存对象或内核代码中的相关缺陷，为优化提供帮助。GPU 在执行命令时，包含了下面四个阶段：

- 主机端将命令提交到命令队列；
- 命令被发送到 GPU 设备端；
- 设备开始执行命令；
- 命令执行完成。

前面介绍的测量方法只能对`函数`的执行时间进行测量，不能更精确的测量`内核`执行时间。如果使用上面的方式来测量内核执行时间，会带来较大的误差。使用 OpenCL 提供的 `Profiling` 操作可以更精确的测量`内核`执行时间，这可以进一步的测算带宽。
