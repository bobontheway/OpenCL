#include <stdio.h>

unsigned long strtoul(const char *ps)
{
	unsigned long result = 0;
	int base = 10;
	int val;

		
	// 1.get base
	if ((ps[0] == '0') && (ps[1] == 'x')) {
		base = 16;
		ps += 2;
	}
	

	// 是否为数字，
	// - 是大于或等于十的十六进制数字？
	// - 十六进制数据又分为大写和小写；

	// 表示方法，
	// 在 c 语言中，十进制直接表示；
	// 八进制前面加 0，例如，对 0 的表示为 00；
	// 十六进制，前面加 0x。
	while (*ps) {
		// 2.get value(Hex or Octo)
		if (((*ps >= 'a') && (*ps <= 'z')) || ((*ps >= 'A') && (*ps <= 'Z')))
		{
			// 2.1 Hex
			val = (*ps | 0x20) - 'a' + 10;
		} else {
			// 2.2 Octo
			val = *ps - '0';
		}

		// val 比 base 还大，表明这是一个错误值。直接返回；
		if (val >= base)
			break;

		// 3.calc
		result = result * base + val;
		ps++;
	}


	return result;
}

int main()
{
	//char *vstr = "123632";
	char *vstr = "0x1987A46";
	//char *vstr = "0x1987b46";

	unsigned long val = strtoul(vstr);

	printf("val = %lu hex val=0x%lx\n", val, val);

	return 0;
}
