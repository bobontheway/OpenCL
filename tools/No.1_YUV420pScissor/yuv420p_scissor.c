#include <stdio.h>
#include <stdlib.h>

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;

/**
 * 将文件中的数据加载到缓冲区中
 */
void load_data(const char *file, uint8_t *addr, uint32_t w, uint32_t h)
{
	FILE *fp = NULL;
	uint32_t size = w * h * 3/2;

	fp = fopen(file, "rb");
	if (NULL != fp) {
		fread(addr, 1, size, fp);
		fclose(fp);
	} else {
		perror("open file fail when load data");
		exit(EXIT_FAILURE);
	}
}

/**
 * 把缓冲区中的数据存入文件
 */
void store_data(const char *file, void *addr, uint32_t w, uint32_t h)
{
	FILE *fp = NULL;
	uint32_t size = w * h * 3/2;

	fp = fopen(file, "wb");
	if(0 != fp) {
		fwrite((void*)addr, 1, size, fp);
		fclose(fp);
	} else {
		perror("open file fail when store data");
		exit(EXIT_FAILURE);
	}
}

/**
 * 提取左上角的图像数据
 */
void yuv420p_left_top(uint8_t *img_buffer, uint8_t *out_buffer,
	int img_orig_width, int img_orig_height, int img_width, int img_height)
{
	int i, j, k = 0;

	int orig_y_size = img_orig_width * img_orig_height,
	    orig_u_size = orig_y_size >> 2;

	/* copy Y section */
	for (i = 0; i < img_height; i++) {
		for (j = 0; j < img_width; j++) {
			out_buffer[k] = img_buffer[i * img_orig_width + j];
			k++;
		}
	}

	/* copy u section */
	for (i = 0; i < img_height/2; i++) {
		for (j = 0; j < (img_width/2); j++) {
			out_buffer[k] = img_buffer[orig_y_size + i * (img_orig_width/2) + j];
			k++;
		}
	}

	/* copy v section */
	for (i = 0; i < img_height/2; i++) {
		for (j = 0; j < (img_width/2); j++) {
			out_buffer[k] = img_buffer[orig_y_size + orig_u_size + i * (img_orig_width/2) + j];
			k++;
		}
	}
}

/**
 * 提取左下角的图像数据（从指定的行开始）
 */
void yuv420p_left_bottom(uint8_t *img_buffer, uint8_t *out_buffer,
	int img_orig_width, int img_orig_height, int img_width, int img_height)
{
	int i, j, k = 0;

	int orig_y_size = img_orig_width * img_orig_height,
	    orig_u_size = orig_y_size >> 2;

	/* copy Y section */
	// height from (img_orig_height - img_height)
	for (i = img_orig_height-img_height; i < img_orig_height; i++) {
		for (j = 0; j < img_width; j++) {
			out_buffer[k] = img_buffer[i * img_orig_width + j];
			k++;
		}
	}

	/* copy u section */
	for (i = (img_orig_height-img_height)/2; i < img_orig_height/2; i++) {
		for (j = 0; j < (img_width/2); j++) {
			out_buffer[k] = img_buffer[orig_y_size + i * (img_orig_width/2) + j];
			k++;
		}
	}

	/* copy v section */
	for (i = (img_orig_height-img_height)/2; i < img_orig_height/2; i++) {
		for (j = 0; j < (img_width/2); j++) {
			out_buffer[k] = img_buffer[orig_y_size + orig_u_size + i * (img_orig_width/2) + j];
			k++;
		}
	}
}

int main()
{
	/* original YUV420p picture */
	int img_orig_width = 3264,
	    img_orig_height = 2448,
	    orig_size = img_orig_width * img_orig_height * 3/2;

#if 0 /* 720p */
	int img_width = 1920,
	    img_height = 1080,
#endif
	    /* size after adjust */
	    int img_width = 1280,
	    img_height = 720,
	    size = img_width * img_height * 3/2;

	/* file names */
	const char *img_file = "./ghost_yuv420p_3264x2448.yuv";
	const char *out_file = "./yuv420p_1280x720.yuv";
	//const char *out_file = "./yuv420p_1920x1080.yuv";

	/* buffer used to save data */
	uint8_t *img_buffer, *out_buffer;

	img_buffer = (uint8_t *)malloc(orig_size);
	out_buffer = (uint8_t *)malloc(size);
	if (!img_buffer || !out_buffer) {
		perror("malloc memory fail");
		exit(EXIT_FAILURE);
	}

	/* load data to buffer from original file */
	load_data(img_file, img_buffer, img_orig_width, img_orig_height);

	/* adjust image size */
	yuv420p_left_bottom(img_buffer, out_buffer, img_orig_width,
		img_orig_height, img_width, img_height);

#if 0 // get left top
	yuv420p_left_top(img_buffer, out_buffer, img_orig_width,
		img_orig_height, img_width, img_height);
#endif

	/* store the data to destinate file */
	store_data(out_file, out_buffer, img_width, img_height);

	/* free buffer */
	free(img_buffer);
	free(out_buffer);

	return 0;
}
