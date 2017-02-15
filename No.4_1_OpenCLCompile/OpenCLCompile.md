## 概述
由于华为 Mate 8 只支持到 OpenCL 1.1，为了使用 OpenCL 1.2 功能，该 OpenCL 程序在 Ubuntu 上执行，GPU 为 AMD RX 460。输出结果如下：

```bash
xbdong@xbdong-opencl:~/Project/github/OpenCL/No.4_1_OpenCLCompile$ ./OpenCLCompile 
[Platform Infomation]
platform name: AMD Accelerated Parallel Processing

[Device Infomation]
device name: Baffin

[Result]
lower case is: hello opencl, i like u
```

## 简介
对于功能比较丰富的程序，在实现的时候，为了便于维护，可以将不同的功能放到不同的头文件件中，再统一编译、链接。在 OpenCL 1.2 中，支持将程序的构建拆分为编译、链接两个步骤，这两个步骤相互独立。

本文描述如何将构建拆分为编译和链接两个独立的步骤，在 `No.4_2_OpenCLCompile` 中再把功能实现放到单独的头文件中。

## 实现

将 `clBuildProgram` 拆分为 `clCompileProgram` 和 `clLinkProgram`。在 `clCompileProgram` 中，参数 `num_input_headers`、`input_headers` 和 `header_include_names` 分别表示程序对象所包含头文件的数目、头文件对应的程序对象，以及头文件的名称，头文件名称和程序对象一一对应。由于这里描述的程序对象没有包含头文件，这些参数分别设置为 0 和NULL 即可，在 `No.4_2_OpenCLCompile` 中会根据头文件的属性来设置相关参数。

### 1.编译
编译 `clCompileProgram` 成功执行完成后，产生的对象文件将和 `program` 程序对象关联，该程序对象在链接阶段使用。如果编译错误，使用 `clGetProgramBuildInfo` 获取编译信息，定位问题所在。
```c
cl_int clCompileProgram (cl_program program,
  	cl_uint num_devices,
  	const cl_device_id *device_list,
  	const char *options,
  	cl_uint num_input_headers,
  	const cl_program *input_headers,
  	const char **header_include_names,
  	void (CL_CALLBACK *pfn_notify)( cl_program program, void *user_data),
  	void *user_data)
```
部分参数说明如下：
- `num_input_headers` - 指定程序对象的数目， 描述 `input_headers` 数组中的头文件；
- `input_headers` - 可看作数组，数组中的程序对象使用 `clCreateProgramWithSource` 创建，程序对象表示内嵌头文件；
- `header_include_names` - 可看作指针数组，其成员和 `input_headers` 中的程序对象一一对应。数组 `header_include_names` 中每个成员指定在 `program` 程序对象中使用引用的内嵌头文件的名字。`input_headers` 中对应的成员表示程序对象，它包含将要使用的头见文件源代码。

### 2.链接
输入的程序对象在经过 clLinkProgram 连接后，生成可执行文件，可在设备端运行。
```c
cl_program clLinkProgram (cl_context context,
  	cl_uint num_devices,
  	const cl_device_id *device_list,
  	const char *options,
  	cl_uint num_input_programs,
  	const cl_program *input_programs,
  	void (CL_CALLBACK *pfn_notify) (cl_program program, void *user_data),
  	void *user_data,
  	cl_int *errcode_ret)
```
部分参数描述如下：
- `num_input_programs` - 指定 `input_programs` 数组中程序对象的数目；
- `input_programs` - 程序对象的数组，被编译为二进制文件或库文件，被链接后创建可执行程序。

下一篇 `No.4_2_OpenCLCompile` 将把功能实现放到单独的头文件中。



























