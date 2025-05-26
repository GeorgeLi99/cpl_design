#ifndef EDGE_H
#define EDGE_H

/**
 * @brief 使用 Sobel 算子进行边缘检测
 * @param data 输入图像数据
 * @param width 图像宽度
 * @param height 图像高度
 * @param channels 图像通道数
 * @param threshold 边缘检测阈值，范围0-255，值越小检测到的边缘越多
 * @return 返回边缘检测结果图像数据，调用者负责释放内存
 */
unsigned char *sobel_edge_detect(const unsigned char *data, int width, int height, int channels, int threshold);

#endif
