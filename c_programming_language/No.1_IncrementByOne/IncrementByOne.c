#include <stdio.h>

int main()
{
	unsigned long long val = 0x0a2a2a0a8a8a0200;
	unsigned int *lo_val = (unsigned int *)&val;

	//unsigned int *hi_val = lo_val++; // ①
	unsigned int *hi_val = lo_val + 1; // ②

	//printf("addr_lo=%p  addr_hi=%p\n", lo_val, hi_val);

	printf("val=0x%.16llx\n\n", val);
	printf("hi_val=0x%.8x\nlo_val=0x%.8x\n", *hi_val, *lo_val);

	return 0;
}
