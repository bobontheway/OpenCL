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

### No.4_OpenCLCompile
把程序构建拆分为编译和链接步骤。Android 下，当前 Mali T-880 只支持到了 OpenCL 1.1。为了使用 OpenCL 1.2 的功能，在 ubuntu 中测试，对应的硬件为 AMD RX460。


