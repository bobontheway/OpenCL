#ifndef __IMAGEALIGN_H__
#define __IMAGEALIGN_H__

#include"common.h"
#include"opencl.h"


int GetAlignImg(uchar **yuvData,int w, int h, uchar imgCnt,uchar *output,char *p_format);
	int SaveImg(char *fileName,uchar *imgData,int w,int h);


#endif
