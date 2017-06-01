## 代码上传
如果本地代码不是最新的，需执行如下操作：

```bash
git push -f origin master
```

### 约定
- 如果是工程项目，全部使用 cpp 后缀的文件名来编码；如果是工具项目，则使用 c
文件编码，特殊情况下可以使用 cpp 文件后缀。

强制执行 push 操作。其中 origin 是远程的 repo 仓库，后面的 master 表示分支。

## OpenCL 编程实践
### No.1_HelloOpenCL
简单的 OpenCL 程序，包含一些基本的操作，便于学习 OpenCL 编程。

### No.2_OpenCLBuild
获取 OpenCL 编码中的错误信息，并对其纠正。

### No.3_OpenCLProgram
将设备端内核代码和主机代码分离，方便程序编写、调试。

### No.4_1_OpenCLCompile
把程序构建拆分为编译和链接步骤。Android 下，当前 Mali T-880 只支持到了 OpenCL 1.1。为了使用 OpenCL 1.2 的功能，在 ubuntu 中测试，对应的硬件为 AMD RX 460。

### No.4_2_OpenCLCompile
在 No.4_1_OpenCLCompile 的基础上，把功能实现放到单独的头头文件中。

### No.5_1_OpenCLRotate
分别在 CPU 上使用 C 语言和在 GPU 上使用 OpenCL 对一张 YUV420p 的图像顺时针旋转 90 度，比较两者之间的性能差异。

### No.5_2_OpenCLRotate
在桌面安装 Ubuntu 系统的 AMD RX 460 上执行，而 No.5_1_OpenCLRotate 在 Android 系统的 ARM T-880 上执行。

### No.6_1_OpenCLSampler
使用 OpenCL 采样器，从原始图像中截取部分矩形区域（左下角区域开始）。

### No.6_2_OpenCLSampler
使用 OpenCL 采样器对图像旋转。

### No.6_3_OpenCLSampler
基于 No.6_2_OpenCLSampler，移植到 Ubuntu 系统中执行。

### No.7_1_OpenCLSyncHost
主机端命令队列同步，clFlush 等操作。

### No.7_2_OpenCLSyncEvent
事件回调函数，根据命令执行状态来完成任务。

### No.7_3_OpenCLSyncEvent
事件同步，不同命令之间同步。

### No.7_4_OpenCLSyncItem
工作项同步，同一个工作组中不同工作项之间同步。

### No.7_5_OpenCLSyncAtomic
原子操作。可以对全局内存区域变量执行原子操作。

### No.7_6_OpenCLAsyncCopy
异步拷贝。在局部内存和全局内存区域之间执行异步拷贝操作。

### No.7_7_OpenCLSyncQueue
提交到命令队列中命令的同步，以及命令队列之间的同步。

### No.8_1_OpenCLTimeCount
使用 Linux 系统提供的时钟/时间接口来获取函数执行的时间戳。
（OpenCLTimeCount.md 包含了对 No.8_2_OpenCLProfiling 和 No.8_3_OpenCLBandwidth 的描述）

### No.8_2_OpenCLProfiling
执行 OpenCL 提供的 Profiling 操作来获取内核运行时间戳。

### No.8_3_OpenCLBandwidth
执行 Profiling 操作来获取时间戳数据，以计算内存拷贝带宽。



