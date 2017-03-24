#include <stdio.h>

int main()
{
	unsigned long long val = 0x0a2a2a0a8a8a0200;
	unsigned int *lo_val = (unsigned int *)&val;

	//unsigned int *hi_val = lo_val++; // ①
	unsigned int *hi_val = lo_val + 1; // ②

	printf("addr_lo=%p  addr_hi=%p\n", lo_val, hi_val);

	printf("val=0x%.16llx\n\n", val);
	printf("hi_val=0x%.8x\nlo_val=0x%.8x\n", *hi_val, *lo_val);

	return 0;
}

// 定义一个 64 位的变量 val，并将它的值初始化为 0x0a2a2a0a8a8a0200；
// 再定义两个指针变量，分别指向 val 的高 32 位和低 32 位值；
// 分别输出两个指针指向地址中存放的内容。

// 如果将 2 中的代码误的写成 1 的代码就会出现高 32 位 和低 32
// 位值颠倒的错误。

// 要记住，自增运算符是对变量的值加 1，然后再将结果赋给原始变量。
// 由于指针变量指向的地址中存放的是整型值，自增后指向连续的下一个整型变量的地址。

// 即，如果指针变量 lo_val 的值为 0x1008，自增后其值为 0x100c；
