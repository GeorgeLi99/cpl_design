#ifndef FILTERS_H
#define FILTERS_H

/**
 * @brief 将图像数据转换为灰度图。
 * @param data 图像的像素数据。
 * @param width 图像的宽度。
 * @param height 图像的高度。
 * @param channels 图像的通道数。
 */
void grayscale(unsigned char *data, int width, int height, int channels);

/**
 * @brief 对图像进行反色处理。
 */
void invert();

/**
 * @brief 对图像应用模糊滤镜。
 */
void blur();

#endif
