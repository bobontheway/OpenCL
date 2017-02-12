## 概述
该 OpenCL 程序在华为 Mate 8 上执行，输出结果如下

```bash
shell@HWFRD:/data/local/tmp $ ./opencl_build
[Platform Infomation]
platform name: ARM Platform

[Device Infomation]
device name: Mali-T880

build log:
<source>:6:28: error: expected ';' after expression
                out[g_id] = in[g_id] + 32
                                         ^

error: Compiler frontend failed (error code 59)
```

## 简介
OpenCL 内核程序在运行之前，需要通过编译、链接等步骤。由于我们的疏忽，程序的编码往往会存在缺陷，导致程序运行异常。其中有些错误在构建时就能确定，这时就要找到导致程序构建错误的地方，再逐一纠正。例如，我们通过修改 No.1_HelloOpenCL，去掉内核程序第 6 行末尾的分号，使其出现编译错误，随后通过 OpenCL API 调用获取错误信息。

## 实现
### 1.修改内核程序
去掉内核程序中第 6 行末尾的分号，使其在构建时失败。
```c
"__kernel void tolower(__global char *in, __global char *out)           \n"
"{                                                                      \n"
"       int g_id = get_global_id(0);                                    \n"
"                                                                       \n"
"       if ((in[g_id] >= 'A') && (in[g_id] <= 'Z'))                     \n"
"               out[g_id] = in[g_id] + 32                               \n"
"       else                                                            \n"
"               out[g_id] = in[g_id];                                   \n"
"}                                                                      \n";
```

### 2.获取构建错误信息
通过下面的 OpenCL API 调用获取构建错误信息。
```c
err = clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG,
        bufSize, buf, NULL);
printf("build log:\n%s\n", buf);
```

### 3.输出日志
输出构建错误的信息，通过对日志信息进行分析，找到编码错误，然后对其纠正。
```bash
build log:
<source>:6:28: error: expected ';' after expression
                out[g_id] = in[g_id] + 32
                                         ^

error: Compiler frontend failed (error code 59)
```
日志中描述这是编译前端错误，对应的错误编码为 59。具体为表达式的后面缺少分号，其中 6 是 program 中的行号，便于快速定位引发错误的位置。

> 注意：
> 上面错误是编译器在编译阶段，语法检查时发现。并非在预处理阶段，预处理阶段主要是头文件包含处理以及宏替换操作。



























