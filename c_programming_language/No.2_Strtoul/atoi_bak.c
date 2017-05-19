#include <stdio.h>

#define true	1
#define	false	0

int isdigit(char ch)
{
	if ((ch >= '0') && (ch <= '9'))
		return true;

	if ((ch >= 'a') && (ch <= 'z'))
		return true;

	if ((ch >= 'A') && (ch <= 'Z'))
		return true;

	return false;
}

int isxdigit(char ch)
{
	if (((ch >= 'a') && (ch <= 'z')) || ((ch >= 'A') && (ch <= 'Z')))
		return true;
	else
		return false;
}

// 十进制、八进制和十六进制
int strtoi(char *ch)
{
	int result = 0;

	// 基数
	int base;

	if (ch[0] == '0') {
		if ((ch[0] == 'x') || (ch[1] == 'X')) {
			// 十六进制
			base = 16;
			ch += 2;
		} else {
			// 八进制
			base = 8;
		}
	} else {
		// 十禁止
		base = 10;
	}

	while (*ch != '\0') {
		if (isdigit(*ch)) {
			if (isxdigit(*ch))
				value = 10 + tolower(*ch) - 'a';
			else
				value = *ch - '0';

			result = result * base + value;
		} else {
			printf("invalide value\n");
			return EXIT_FAILURE;
		}
	}
	return result;
}

// 转换为小写字母
int inline tolower(char ch)
{
	return (ch | 0x20);
}

// 十六进制
int htoi(char *ch)
{
	int result = 0;

	if (ch[0] == '0') {
		if ((ch[1] == 'x') || (ch[1] == 'X'))
			ch += 2;
	}

	while (*ch != '\0') {
		if (isdigit(*ch)) {
			if (isxdigit(*ch))
				value = 10 + tolower(*ch) - 'a';
			else
				value = *ch - '0';

			result = result * 16 + value;
		} else {
			printf("invalide value\n");
			return EXIT_FAILURE;
		}
	}
	return result;
}

int main(int argc, char *argv[])
{
	return 0;
}
