## 概述
该 OpenCL 程序在华为 Mate 8 上执行，输出结果如下

```bash
shell@HWFRD:/data/local/tmp $ ./opencl_program
[Platform Infomation]
platform name: ARM Platform

[Device Infomation]
device name: Mali-T880

[Result]
lower case is: hello opencl, i like u
```

## 简介
为了便于设备端 OpenCL 代码的编辑和调试，在 `No.1_HelloOpenCL` 的基础上，将设备端的内核代码从主机代码分离，存放到单独的 `program.cl` 文件中。这样主机代码在无需重新编译的情况下，只修改 `program.cl` 文件，就可以重新定义内核代码功能。

## 实现
### 1.分离内核代码
将下面 OpenCL 代码从源文件分离出来，存放到新建的 `program.cl` 文件中。
```c
__kernel void tolower(__global char *in, __global char *out)
{
	int g_id = get_global_id(0);

	if ((in[g_id] >= 'A') && (in[g_id] <= 'Z'))
		out[g_id] = in[g_id] + 32;
	else
		out[g_id] = in[g_id];
}
```

### 2.实现 package_program 函数
在主机端实现 package_program 函数，其目的是将文件中的内容以字符串的形式存放到缓冲区中。
```c
char *package_program(const char *filename)
{
	FILE *file;
	char *buf;
	long program_size;

	file = fopen(filename, "rb");
	if (!file) {
		perror("open file fail");
		return NULL;
	}

	// 设置文件位置指示符，指向文件末尾
	fseek(file, 0, SEEK_END);

	// 获取文件指示符的当前位置
	program_size = ftell(file);

	// 重置指示符指向文件的起始位置
	rewind(file);

	buf = (char *)malloc(program_size + 1);
	if (!buf) {
		perror("alloc memory fail");
		fclose(file);
		return NULL;
	}
	buf[program_size] = '\0';
	fread(buf, sizeof(char), program_size, file);
	fclose(file);
	return buf;
}
```

文件中每行的末尾自动补充一个换行符 `\n`，其 ASCII 码对应的值为 `0xA`。调用 malloc 分配缓冲区的时候，会多分配一个字节内存单元，向其中存入 `\0` 字符。这样在调用 `clCreateProgramWithSource` 的时候，参数 `lengths` 可直接传入 NULL，表示 `strings` 参数是以 null 结尾的字符串。

### 3.创建程序对象
使用缓冲区 `program_buf` 作为参数，创建程序对象。程序对象创建后，可以释放该缓冲区。
```c
program = clCreateProgramWithSource(context, 1, (const char **)&program_buf, NULL, &err);
```
##参考
http://man7.org/linux/man-pages/man3/fseek.3.html



























