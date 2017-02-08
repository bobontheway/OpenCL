
#define IMAGE_FORMAT  "YVU420_SEMIPLANAR"//"YUV420_PLANAR"//
//#define IMAGE_FORMAT  "YUV420_PLANAR"
#include "interface.h"
#include"cmr_common.h"

#if 1
#include <sys/time.h>
#include <time.h>

static long long gTime;
//unsigned char p[300];

static long long system_time()
{
	struct timespec t;
	t.tv_sec = t.tv_nsec = 0;
	clock_gettime(CLOCK_MONOTONIC, &t);
	return (long long)(t.tv_sec)*1000000000LL + (long long)t.tv_nsec;
}

static void startTime()
{
	gTime = system_time();
}

static void endTime(const char *str)
{
	long long t = system_time() - gTime;
	double ds = ((double)t) / 1e6;
	printf("Test: %s, %f ms\n", str, ds);

}
#endif

static void input_data(uint width, uint height, const char *file, uchar *addr)
{
	FILE *fp = NULL;
	uint size = width * height * 3/2;

	fp = fopen(file, "rb");
	if (NULL != fp) {
		fread(addr, 1, size, fp);
		fclose(fp);
	} else {
		printf("can not create savedata");
	}
}

static void save_rotate_data(void *addr, uint width, uint height, const char *file)
{
	FILE *fp = NULL;
	uint size = width*height*3/2;

	fp = fopen(file, "wb");
	if(0 != fp) {
		fwrite((void*)addr, 1, size, fp);
		fclose(fp);
	}else{
		printf("can not create savedata");
	}
}
int main(void)
{
	uint width=3264;//=3264;//3280;//3456;
	uint height=2448;//=2448;//2464;//2304;
	uint size;//= width * height * 3/2;
	uchar *inBuf;
	uchar *outBuf;
	const char *inFile = "/data/local/tmp/3264x2448dghost-1_yuv420_vu.yuv";
	const char *outFile = "/data/local/tmp/rotate_test.yuv";
	int ret=0;

	// alloc memory
	size = width * height * 3/2;
	inBuf = (uchar*)malloc(size);
	outBuf = (uchar*)malloc(size);
	if ((NULL == inBuf) || (NULL == outBuf)) {
		ret = -1;
		printf("malloc fail.");
	}

	// file -> buffer
	input_data(width, height, inFile, inBuf);

	startTime();
	if (0 != Rotate_Function(inBuf, outBuf, height, width))
	{
		CMR_LOGE("rotate function error!");
		ret=-1;
	}
	endTime("Rotate_Function");

	save_rotate_data(outBuf, width, height, outFile);
	free(inBuf);
	free(outBuf);

	return ret;
}



