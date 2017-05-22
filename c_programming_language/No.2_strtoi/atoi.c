#include <stdio.h>
#include <stdlib.h>

#define true	1
#define false	0

// 字符是否为十六进制数字
int isDigit(char ch)
{
	if ((ch >= '0') && (ch <= '9'))
		return true;

	if ((ch >= 'a') && (ch <= 'f'))
		return true;

	if ((ch >= 'A') && (ch <= 'F'))
		return true;

	return false;
}

// 字符为 'a' ~ 'h' 或 ‘A’ ~ 'H' 的十六进制数字
int isXDigit(char ch)
{
	if (((ch >= 'a') && (ch <= 'f')) || ((ch >= 'A') && (ch <= 'F')))
		return true;
	else
		return false;
}

// 转换为小写字母
int inline toLower(char ch)
{
	return (ch | 0x20);
}

// 十六进制
int htoi(const char *ch)
{
	int result = 0;
	int value;

	if (ch[0] == '0') {
		if ((ch[1] == 'x') || (ch[1] == 'X'))
			ch += 2;
	}

	while (*ch != '\0') {
		if (isDigit(*ch)) {
			if (isXDigit(*ch))
				value = 10 + toLower(*ch) - 'a';
			else
				value = *ch - '0';

			result = result * 16 + value;
		} else {
			printf("invalide value\n");
			return EXIT_FAILURE;
		}
		ch++;
	}
	return result;
}

int main(int argc, char *argv[])
{
	char *vstr = "0x1987A46";

	int val = htoi(vstr);
	printf("val = %d hex val=0x%x\n", val, val);
	return 0;
}
