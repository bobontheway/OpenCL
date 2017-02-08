// Right Rotate 90 degree
void rotateYUV420SP_normal_pUV(unsigned char *src, unsigned char *des,
	int w, int h)
{
	int i, j;
	int wh = w * h;

	// 旋转 Y 分量（名亮度，也就是灰度值）
	int k = 0;  
	for(i = 0; i < w; i++) {
		//for(j = 0; j < height; j++)   
		for(j = h-1; j >= 0; j--)   
		{  
			des[k] = src[w*j + i];
			k++;  
		}  
	}  

	// 旋转 UV 分量（色度，也就是影像色彩及饱和度，指定像素颜色）
	// 假设 UV 是按照 p 格式非连续排列的
	// init U
	for (j = 0; j < w/2; j++) {
		for (i = 1; i <= h/2; i++) {
			//outUV[k] = (h/2 - i) * (w/2) + j;
			des[k] = src[wh + ((h/2 - i) * (w/2) + j)];
			k++;
		}
	}

	// init V
	for (j = 0; j < w/2; j++) {
		for (i = 1; i <= h/2; i++) {
			//outUV[k] = (h/2 - i) * (w/2) + j;
			des[k] = src[wh+wh/4 + ((h/2 - i) * (w/2) + j)];
			k++;
		}
	}
}

// Right Rotate 90 degree
void rotateYUV420SP_normal_pUV_OpenCL(unsigned char *src, unsigned char *des,
	int w, int h)
{
	int i, j;
	int wh = w * h;

	// 旋转 Y 分量（名亮度，也就是灰度值）
	int k = 0;  
	for(i = 0; i < w; i++) {
		//for(j = h-1; j >= 0; j--)   /*
		//循环是可以颠倒的，上下结果一致 */
		for(j = 0; j < h; j++)   
		{  
			//des[k] = src[w*j + i];
			//k++;
			des[i * h + (h-1)-j] = src[w*j + i];

#if 0 /* debug */
			if ((k % 10) == 0)
				printf("\n");
			printf("k=%.6d  ", (i * h + (h-1)-j));
			k++;
#endif
		}  
	}  

	// 旋转 UV 分量（色度，也就是影像色彩及饱和度，指定像素颜色）
	// 假设 UV 是按照 p 格式非连续排列的
	// init U
	for (i = 0; i < w/2; i++) {
		for (j = 1; j <= h/2; j++) {
			//outUV[k] = (h/2 - j) * (w/2) + i;
			//des[k] = src[wh + ((h/2 - j) * (w/2) + i)];
			//k++;
			des[wh + (i * h/2 + (j-1))] = src[wh + ((h/2 - j) * (w/2) + i)];
		}
	}

	// init V
	for (i = 0; i < w/2; i++) {
		for (j = 1; j <= h/2; j++) {
			//outUV[k] = (h/2 - j) * (w/2) + i;
			//des[k] = src[wh+wh/4 + ((h/2 - j) * (w/2) + i)];
			//k++;
			des[(wh+wh/4) + (i * h/2 + (j-1))] = src[wh+wh/4 + ((h/2 - j) * (w/2) + i)];
		}
	}
}

// Right Rotate 90 degree
// 将除法变为移位操作
void rotateYUV420SP_normal_pUV_performance1(unsigned char *src, unsigned char *des,
	int w, int h)
{
	int i, j;
	int wh = w * h;

	// 旋转 Y 分量（名亮度，也就是灰度值）
	int k = 0;  
	for(i = 0; i < w; i++) {
		for(j = h-1; j >= 0; j--)   
		{  
			des[k] = src[w*j + i];
			k++;  
		}  
	}  

	// 旋转 UV 分量（色度，也就是影像色彩及饱和度，指定像素颜色）
	// 假设 UV 是按照 p 格式非连续排列的
	// init U
	for (j = 0; j < w>>1; j++) {
		for (i = 1; i <= (h>>1); i++) {
			des[k] = src[wh + (((h>>1) - i) * (w>>1) + j)];
			k++;
		}
	}

	// init V
	for (j = 0; j < w>>1; j++) {
		for (i = 1; i <= h>>1; i++) {
			des[k] = src[wh+(wh>>2) + (((h>>1) - i) * (w>>1) + j)];
			k++;
		}
	}
}

// Right Rotate 90 degree
// 把循环中的移位操作去掉
void rotateYUV420SP_normal_pUV_performance2(unsigned char *src, unsigned char *des,
	int w, int h)
{
	int i, j;
	int wh = w * h;

	// 旋转 Y 分量（名亮度，也就是灰度值）
	int k = 0;  
	for(i = 0; i < w; i++) {
		for(j = h-1; j >= 0; j--)   
		{  
			des[k] = src[w*j + i];
			k++;  
		}  
	}  

	// 旋转 UV 分量（色度，也就是影像色彩及饱和度，指定像素颜色）
	// 假设 UV 是按照 p 格式非连续排列的
	// init U
	int halfH = h >> 1,
	    halfW = w >> 1,
	    halfUV = wh >> 2;

	for (j = 0; j < halfW; j++) {
		for (i = 1; i <= halfH; i++) {
			des[k] = src[wh + ((halfH - i) * halfW + j)];
			k++;
		}
	}

	// init V
	for (j = 0; j < halfW; j++) {
		for (i = 1; i <= halfH; i++) {
			des[k] = src[wh+halfUV + ((halfH - i) * halfW + j)];
			k++;
		}
	}
}
