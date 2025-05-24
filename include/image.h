#ifndef IMAGE_H
#define IMAGE_H

/**
 * @brief 从指定路径加载图像。
 * @param path 图像文件的路径。
 * @param width 指向存储图像宽度的变量的指针。
 * @param height 指向存储图像高度的变量的指针。
 * @param channels 指向存储图像通道数的变量的指针。
 * @return 成功返回图像数据指针，失败返回NULL。
 */
unsigned char *load_image(const char *path, int *width, int *height, int *channels);

/**
 * @brief 将图像保存到指定路径。
 * @param path 保存图像文件的路径。
 * @param data 图像数据。
 * @param width 图像宽度。
 * @param height 图像高度。
 * @param channels 图像通道数。
 * @param quality JPEG质量参数 (1-100)，仅对JPEG格式有效。
 * @return 成功返回1，失败返回0。
 */
int save_image(const char *path, unsigned char *data, int width, int height, int channels, int quality);

#endif
