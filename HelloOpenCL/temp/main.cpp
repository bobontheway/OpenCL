
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
	double ds = ((double)t) / 1e9;
	printf("Test: %s, %f s\n", str, ds);//CMR_LOGI("Test: %s, %f s\n", str, ds);

}
#endif

static void input_data(uint width,uint height,char *file1,char *file2,char *file3,uchar *addr0,uchar *addr1,uchar *addr2)
{
	FILE *fp = NULL;
	uint size = width*height*3/2;

	fp = fopen(file1, "rb");
	if(0 != fp) {
		fread(addr0, 1, size, fp);
		fclose(fp);
	}else{
		printf("can not create savedata");
	}
	fp = fopen(file2, "rb");
	if(0 != fp) {
		fread(addr1, 1, size, fp);
		fclose(fp);
	}else{
		printf("can not create savedata");
	}
	fp = fopen(file3, "rb");
	if(0 != fp) {
		fread(addr2, 1, size, fp);
		fclose(fp);
	}else{
		printf("can not create savedata");
	}
}
static void save_hdrdata(void *addr,uint width,uint height,char *file)
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
int main(int argc, char **argv)
{
	uint width=3264;//=3264;//3280;//3456;
	uint height=2448;//=2448;//2464;//2304;
	uint size;//= width * height * 3/2;
	uchar *img[3];
	uchar *output;
	char  *p_format = IMAGE_FORMAT;
	char *in1="/data/local/tmp/3264x2448dghost-1_yuv420_vu.yuv";
	char *in2="/data/local/tmp/3264x2448dghost-2_yuv420_vu.yuv";
	char *in3="/data/local/tmp/3264x2448dghost-3_yuv420_vu.yuv";
	char *out="/data/local/tmp/test.yuv";
	int ret=0;

	//printf("james width=%d,height=%d\n",width,height);
	size = width * height * 3/2;
	img[0] = (uchar*)malloc(size);
	img[1] = (uchar*)malloc(size);
	img[2] = (uchar*)malloc(size);
	output = (uchar*)malloc(size);
	if ((NULL==img[0]) || (NULL==img[1])|| (NULL==img[2])|| (NULL==out))
	{
		ret = -1;
		printf("malloc fail.");
	}

	input_data(width,height,in1,in2,in3,img[0],img[1],img[2]);
startTime();
	if (0 != HDR_Function(img[0], img[1], img[2], img[0], height, width, p_format))
	{
		CMR_LOGE("hdrfuction error!");
		ret=-1;
	}
endTime("HDR_Function");

	save_hdrdata(img[0],width,height,out);
	free(img[0]);
	free(img[1]);
	free(img[2]);
	free(output);

	return ret;
}



