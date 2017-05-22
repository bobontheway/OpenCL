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

// 十六进制、八进制、十进制
int strtoi(char *ch)
{
	int value;
	int result = 0;

	int base; // 基数

	if (ch[0] == '0') {
		if ((ch[1] == 'x') || (ch[1] == 'X')) {
			base = 16; // 十六进制
			ch += 2;
		} else {
			base = 8; // 八进制
		}
	} else {
		base = 10; // 十禁止
	}

	printf("base = %d\n", base);
	while (*ch != '\0') {
		if (isDigit(*ch)) {
			if (isXDigit(*ch))
				value = 10 + toLower(*ch) - 'a';
			else
				value = *ch - '0';

			if (value >= base) {
				printf("invalide value\n");
				return EXIT_FAILURE;
			}

			result = result * base + value;
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
	//char *vstr = "017246";
	//char *vstr = "0X1987A46";
	char *vstr = "198746";

	int val = strtoi(vstr);
	printf("val = %d hex val=0x%x\n", val, val);
	return 0;
}
