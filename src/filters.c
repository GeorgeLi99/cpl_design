#include "filters.h"
#include <stddef.h> // For size_t

/**
 * @brief 将图像数据转换为灰度图。
 * @param data 图像的像素数据。
 * @param width 图像的宽度。
 * @param height 图像的高度。
 * @param channels 图像的通道数。
 */
void grayscale(unsigned char *data, int width, int height, int channels)
{
    if (data == NULL || width <= 0 || height <= 0 || channels <= 0) {
        // 可以选择在这里添加错误处理或日志记录
        return;
    }
    size_t pixel_count = (size_t)width * height;
    for (size_t i = 0; i < pixel_count; i++) {
        unsigned char *p = data + i * channels;
        // 计算灰度值 (Luminance method)
        unsigned char gray = (unsigned char)(0.299f * p[0] + 0.587f * p[1] + 0.114f * p[2]);
        p[0] = gray; // R
        p[1] = gray; // G
        p[2] = gray; // B
        // 如果有alpha通道，通常保持不变
        // if (channels == 4) { p[3] = p[3]; }
    }
}

/**
 * @brief 对图像进行反色处理。
 */
void invert() { }

/**
 * @brief 对图像应用模糊滤镜。
 */
void blur() { }
