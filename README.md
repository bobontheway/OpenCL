## 代码上传
如果本地代码不是最新的，需执行如下操作：

```bash
git push -f origin master
```

强制执行 push 操作。其中 origin 是远程的 repo 仓库，后面的 master 表示分支。

## 示例说明
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

### No.6_OpenCLSampler
使用 OpenCL 采样器对图像旋转。

### No.6_2_OpenCLSampler
基于 No.6_OpenCLSampler，移植到 Ubuntu 系统中执行
