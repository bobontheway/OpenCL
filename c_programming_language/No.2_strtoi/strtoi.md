# 字符串转整型值
计算机中的数字使用二进制来表示，但实际很多情况直接使用二进制来描述很不方便。在生活中，我们习惯使用十进制来表述，但它却很难和计算机直接关联。为此，引入了十六进制作为二进制和十进制之间的过渡。四位二进制数对应一位十六进制数，这在描述某些信息，如地址时，可以更方便结合计算机结构来理解。八进制对应三个二进制位，相对与十六进制，在计算机中使用的较少。

## 概述
当用户和计算机进行交互时，有时需要将输入的字符串转换为整数交给计算机处理。本文首先完成 `The C Programming Language` 中的练习 `2-3`， 将十六进制数字组成的字符串转换为与之等价的整型值。接着对该练习进行展开，将字符串转换为与之等价的整型值，该字符串可能由十进制数字、八进制数字或十六进制数字组成。在 c 语言中，八进制数的表示需要在前面加 0，十六进制表示需要在前面加 `0x` 或 `0X`，十进制直接表示。

## 练习 2-3
编写函数 `htoi(s)`，把由十六进制数字组成的字符串（包含的前缀是可选的，可以是 `0x` 或 `0X`）转换为与之等价的整型值。字符串中允许包含的数字包括：`0～9`、`a~f` 以及 `A~F`。

源代码实现如下：
```c
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
```

## 字符串转整型值
在 `练习 2-3` 的基础上展开，实现 `strtoi`，将字符串转换为与之等价的整型值，该字符串可能由十进制数字、八进制数字或十六进制数字组成。在 c 语言中，八进制数的表示需要在前面加 0，十六进制表示需要在前面加 `0x` 或 `0X`，十进制直接表示。相关代码如下：
```c
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
```
完整代码参见 [ss](ss);

## 参考

- The C Programming Language
- https://zhidao.baidu.com/question/61885883.html