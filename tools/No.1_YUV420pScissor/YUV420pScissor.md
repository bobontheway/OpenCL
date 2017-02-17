## 概述
在 [No.5_OpenCLRotate](../../No.5_OpenCLRotate/OpenCLRotate.md) 中对一张大小为 3264x2448 的 YUV420p 图像旋转后，由于显示器的分辨率只有 1920x1080，不能完全显示整张图像，于是不能直观的确定旋转正确与否。为了进一步确认旋转算法的正确性，将原始图像裁剪为较小的图像。裁剪方法是从原始图像的左上角或左下角切割一块期望大小的图像。

## 实现
### 1.提取左上角
从左上角开始，提取一块矩形区域的像素，作为 YUV420p 图形格式。如下图所示，分别提取原始图像的 Y、U 和 V 分量，使用黑框圈起来白色字体描述。

<img src="image/scissor_left_top.png" width="60%" height="60%">

下面是使用 c 语言实现该算法的相关代码：
```c
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
```

### 2.提取左下角
提取左下角一块矩形区域的像素，作为 YUV420p 图形格式。如下图所示，分别提取原始图像的 Y、U 和 V 分量，使用黑框圈起来白色字体描述。

<img src="image/scissor_left_bottom.png" width="60%" height="60%">

下面是使用 c 语言实现该算法的相关代码：
```c
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
```



























