## 概述
本文是上一篇 `No.4_1_OpenCLCompile` 的第二部分，把功能实现放到单独的头文件中。输出结果如下：

```bash
xbdong@xbdong-opencl:~/Project/github/OpenCL/No.4_2_OpenCLCompile$ ./OpenCLCompile
[Platform Infomation]
platform name: AMD Accelerated Parallel Processing

[Device Infomation]
device name: Baffin

[Result]
lower case is: hello opencl, i like u
```

## 实现
### 1.创建头文件
新建 `lower.cl` 文件，在该文件中添加 `lower` 函数，该函数实现将大写字母转换为小写字母。
```c
char lower(char val)
{
	return val+32;
}
```

### 2.头文件程序对象
创建头文件对应的程序对象，在编译时使用。
```c
cl_program header_program;

/* header program */
program_buf = package_program("lower.cl");
if (!program_buf) {
	printf("alloc program buffer fail:lower.cl\n");
	exit(EXIT_FAILURE);
}

// create program
header_program = clCreateProgramWithSource(context, 1, (const char **)&program_buf, NULL, &err);
if (header_program == NULL) {
	printf("create header program fail\n");
	exit(EXIT_FAILURE);
}
free(program_buf);
```

### 3.编译

```c
err = clCompileProgram(program, 1, &device, NULL,
	1, &header_program, &header_name, NULL, NULL);
```
相关参数在 [No.4_1_OpenCLCompile](../No.4_1_OpenCLCompile/OpenCLCompile.md) 中描述，这里不在赘述。编译后的程序对象 `program` 在链接时使用。由于在链接时，只有一个程序对象输入，这和 `No.4_1_OpenCLCompile` 中一样，故无需对 `clLinkProgram` 的函数参数进行修改。
