#ifndef ASCII_ART_H
#define ASCII_ART_H

/**
 * @brief 将图像转换为 ASCII 字符画并保存到文件中。
 * @param data 图像数据。
 * @param width 图像宽度。
 * @param height 图像高度。
 * @param channels 图像通道数。
 * @param output_file 输出ASCII字符画的文件路径。
 * @param scale_factor 缩放因子，用于调整输出字符画的大小，值越大输出越小。
 * @return 成功返回1，失败返回0。
 */
int image_to_ascii(unsigned char *data, int width, int height, int channels, const char *output_file, int scale_factor);

#endif
