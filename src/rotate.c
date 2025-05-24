#include "rotate.h"
#include <stdlib.h> // 为 malloc 和 free 函数
#include <string.h> // 为 memcpy 函数

/**
 * @brief 旋转图像（实现180度翻转）
 * @param data 图像的像素数据
 * @param width 图像宽度
 * @param height 图像高度
 * @param channels 图像通道数
 */
void rotate_image(unsigned char *data, int width, int height, int channels)
{
    if (!data || width <= 0 || height <= 0 || channels <= 0)
        return;

    // 分配临时缓冲区
    unsigned char *temp = (unsigned char *)malloc(width * height * channels);
    if (!temp)
        return;

    // 复制原始数据
    memcpy(temp, data, width * height * channels);

    // 垂直翻转 (上下翻转)
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // 源像素位置：从顶部开始
            unsigned char *src_pixel = temp + (y * width + x) * channels;
            // 目标像素位置：从底部开始
            unsigned char *dst_pixel = data + ((height - 1 - y) * width + x) * channels;

            // 复制所有通道的像素值
            for (int c = 0; c < channels; c++) {
                dst_pixel[c] = src_pixel[c];
            }
        }
    }

    // 释放临时缓冲区
    free(temp);
}
