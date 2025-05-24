#ifndef ROTATE_H
#define ROTATE_H

/**
 * @brief 旋转图像（实现180度翻转）
 * @param data 图像的像素数据
 * @param width 图像宽度
 * @param height 图像高度
 * @param channels 图像通道数
 */
void rotate_image(unsigned char *data, int width, int height, int channels);

#endif