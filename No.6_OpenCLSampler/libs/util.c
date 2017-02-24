#include "../include/util.h"

/**
 * 将文件中的数据加载到缓冲区中
 */
int load_data(const char *file, uint8_t *addr, uint32_t size)
{
	int ret = 0;
	FILE *fp = NULL;

	fp = fopen(file, "rb");
	if (NULL != fp) {
		fread(addr, 1, size, fp);
		fclose(fp);
	} else {
		perror("open file fail when load data");
		ret = EXIT_FAILURE;
	}

	return ret;
}

/**
 * 把缓冲区中的数据存入文件
 */
int store_data(const char *file, void *addr, uint32_t size)
{
	int ret = 0;
	FILE *fp = NULL;

	fp = fopen(file, "wb");
	if(0 != fp) {
		fwrite((void*)addr, 1, size, fp);
		fclose(fp);
	} else {
		perror("open file fail when store data");
		ret = EXIT_FAILURE;
	}
	return ret;
}
