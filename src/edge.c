#include "edge.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

/**
 * @brief 使用 Sobel 算子进行边缘检测
 * @param data 输入图像数据
 * @param width 图像宽度
 * @param height 图像高度
 * @param channels 图像通道数
 * @param threshold 边缘检测阈值，范围0-255，值越小检测到的边缘越多
 * @return 返回边缘检测结果图像数据，调用者负责释放内存
 */
unsigned char *sobel_edge_detect(const unsigned char *data, int width, int height, int channels, int threshold)
{
    if (!data || width <= 0 || height <= 0 || channels <= 0) {
        fprintf(stderr, "Invalid parameters for sobel_edge_detect\n");
        return NULL;
    }

    // 确保阈值在有效范围内
    if (threshold < 0)
        threshold = 0;
    if (threshold > 255)
        threshold = 255;

    // 为边缘检测结果分配内存（与输入图像相同大小）
    unsigned char *edge_data = (unsigned char *)malloc(width * height * channels);
    if (!edge_data) {
        fprintf(stderr, "Memory allocation failed in sobel_edge_detect\n");
        return NULL;
    }

    // 初始化边缘图像为黑色
    memset(edge_data, 0, width * height * channels);

    // 创建灰度图，用于计算边缘
    unsigned char *gray_data = (unsigned char *)malloc(width * height);
    if (!gray_data) {
        fprintf(stderr, "Memory allocation failed for gray image\n");
        free(edge_data);
        return NULL;
    }

    // 转换为灰度图
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int pixel_idx = (y * width + x) * channels;

            if (channels >= 3) {
                // 使用加权平均计算灰度值（相同的灰度转换公式）
                float gray_value = 0.299f * data[pixel_idx] +     // R
                                   0.587f * data[pixel_idx + 1] + // G
                                   0.114f * data[pixel_idx + 2];  // B
                gray_data[y * width + x] = (unsigned char)gray_value;
            }
            else {
                // 已经是灰度图
                gray_data[y * width + x] = data[pixel_idx];
            }
        }
    }

    // Sobel算子
    // Gx: 水平梯度算子
    // [-1 0 1]
    // [-2 0 2]
    // [-1 0 1]
    //
    // Gy: 垂直梯度算子
    // [-1 -2 -1]
    // [ 0  0  0]
    // [ 1  2  1]

    // 创建临时的梯度幅值图像，用于进一步处理
    unsigned char *magnitude_data = (unsigned char *)malloc(width * height);
    if (!magnitude_data) {
        fprintf(stderr, "Memory allocation failed for magnitude data\n");
        free(gray_data);
        return edge_data;
    }

    // 初始化为0
    memset(magnitude_data, 0, width * height);

    // 找出最大梯度幅值，用于归一化
    int max_magnitude = 0;

    // 应用Sobel算子并计算所有像素的梯度幅值
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            // 计算水平梯度 Gx
            int gx = (-1) * gray_data[(y - 1) * width + (x - 1)] + (0) * gray_data[(y - 1) * width + (x)] +
                     (1) * gray_data[(y - 1) * width + (x + 1)] + (-2) * gray_data[(y)*width + (x - 1)] +
                     (0) * gray_data[(y)*width + (x)] + (2) * gray_data[(y)*width + (x + 1)] +
                     (-1) * gray_data[(y + 1) * width + (x - 1)] + (0) * gray_data[(y + 1) * width + (x)] +
                     (1) * gray_data[(y + 1) * width + (x + 1)];

            // 计算垂直梯度 Gy
            int gy = (-1) * gray_data[(y - 1) * width + (x - 1)] + (-2) * gray_data[(y - 1) * width + (x)] +
                     (-1) * gray_data[(y - 1) * width + (x + 1)] + (0) * gray_data[(y)*width + (x - 1)] +
                     (0) * gray_data[(y)*width + (x)] + (0) * gray_data[(y)*width + (x + 1)] +
                     (1) * gray_data[(y + 1) * width + (x - 1)] + (2) * gray_data[(y + 1) * width + (x)] +
                     (1) * gray_data[(y + 1) * width + (x + 1)];

            // 计算梯度幅值
            int magnitude = (int)sqrt(gx * gx + gy * gy);

            // 记录最大梯度幅值
            if (magnitude > max_magnitude) {
                max_magnitude = magnitude;
            }

            // 存储梯度幅值
            magnitude_data[y * width + x] = (magnitude > 255) ? 255 : magnitude;
        }
    }

    // 应用非极大值抑制和双阈值（简化版）
    int high_threshold = threshold;
    int low_threshold = threshold / 2;

    // 应用阈值，生成边缘图像
    for (int y = 1; y < height - 1; y++) {
        for (int x = 1; x < width - 1; x++) {
            int mag = magnitude_data[y * width + x];

            // 强边缘 - 直接标记为白色
            if (mag > high_threshold) {
                for (int c = 0; c < channels; c++) {
                    edge_data[(y * width + x) * channels + c] = 255;
                }
            }
            // 弱边缘 - 如果连接到强边缘，也标记为白色
            else if (mag > low_threshold) {
                // 检查8邻域是否存在强边缘
                bool connected_to_strong = false;
                for (int ny = -1; ny <= 1 && !connected_to_strong; ny++) {
                    for (int nx = -1; nx <= 1 && !connected_to_strong; nx++) {
                        if (nx == 0 && ny == 0)
                            continue;

                        int neighbor_mag = magnitude_data[(y + ny) * width + (x + nx)];
                        if (neighbor_mag > high_threshold) {
                            connected_to_strong = true;
                            break;
                        }
                    }
                }

                if (connected_to_strong) {
                    for (int c = 0; c < channels; c++) {
                        edge_data[(y * width + x) * channels + c] = 255;
                    }
                }
            }
            // 非边缘保持为黑色
        }
    }

    // 释放临时图像数据
    free(gray_data);
    free(magnitude_data);

    printf("Sobel edge detection with hysteresis completed (threshold: %d)\n", threshold);
    return edge_data;
}
