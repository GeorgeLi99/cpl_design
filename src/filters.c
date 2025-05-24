#include "filters.h"
#include <stddef.h> // For size_t
#include <stdlib.h> // For malloc and free
#include <string.h> // For memcpy

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
 * @param data 图像的像素数据。
 * @param width 图像的宽度。
 * @param height 图像的高度。
 * @param channels 图像的通道数。
 */
void invert(unsigned char *data, int width, int height, int channels)
{
    if (data == NULL || width <= 0 || height <= 0 || channels <= 0) {
        return; // 参数无效，直接返回
    }

    // 计算总像素数
    size_t pixel_count = (size_t)width * height;

    // 遍历所有像素
    for (size_t i = 0; i < pixel_count; i++) {
        unsigned char *p = data + i * channels;

        // 反转 RGB 分量 (255 - 原值)
        for (int j = 0; j < channels && j < 3; j++) {
            p[j] = 255 - p[j];
        }

        // 如果有 Alpha 通道（第4个通道），保持不变
        // 通常我们不会反转 Alpha 通道，因为它表示透明度
    }
}

/**
 * @brief 对图像应用模糊滤镜。
 * @param data 图像的像素数据。
 * @param width 图像的宽度。
 * @param height 图像的高度。
 * @param channels 图像的通道数。
 * @param radius 模糊半径，值越大模糊效果越强。
 */
void blur(unsigned char *data, int width, int height, int channels, int radius)
{
    if (data == NULL || width <= 0 || height <= 0 || channels <= 0 || radius <= 0) {
        return; // 参数无效，直接返回
    }

    // 为处理后的图像分配临时内存
    unsigned char *temp = (unsigned char *)malloc(width * height * channels);
    if (temp == NULL) {
        // 内存分配失败
        return;
    }

    // 复制原始数据到临时缓冲区
    memcpy(temp, data, width * height * channels);

    // 注释掉未使用的变量
    // int kernel_size = 2 * radius + 1;

    // 为了简化，使用均值模糊而不是真正的高斯模糊
    // 每个像素取周围区域的平均值

    // 遍历所有像素
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // 对每个通道分别计算模糊值
            for (int c = 0; c < channels; c++) {
                int sum = 0;
                int count = 0;

                // 遍历核心区域
                for (int ky = -radius; ky <= radius; ky++) {
                    for (int kx = -radius; kx <= radius; kx++) {
                        // 计算相邻像素坐标
                        int nx = x + kx;
                        int ny = y + ky;

                        // 边界检查 - 使用边界钳制方法
                        if (nx < 0)
                            nx = 0;
                        if (nx >= width)
                            nx = width - 1;
                        if (ny < 0)
                            ny = 0;
                        if (ny >= height)
                            ny = height - 1;

                        // 获取相邻像素值并累加
                        sum += temp[(ny * width + nx) * channels + c];
                        count++;
                    }
                }

                // 计算平均值并设置到目标像素
                data[(y * width + x) * channels + c] = (unsigned char)(sum / count);
            }
        }
    }

    // 释放临时内存
    free(temp);
}
