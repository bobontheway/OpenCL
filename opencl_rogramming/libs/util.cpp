#include "util.h"

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

/**
 * 计算程序执行时间
 */
static int64_t g_time;

static int64_t system_time()
{
	struct timespec t;
	t.tv_sec = t.tv_nsec = 0;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return (int64_t)(t.tv_sec)*1000000000LL + (int64_t)t.tv_nsec;
}

void time_start(void)
{
	g_time = system_time();
}

void time_end(const char *str)
{
	int64_t t = system_time() - g_time;
	double ds = ((double)t) / 1e3;
	printf("Test: %s, %f us\n", str, ds);

}
