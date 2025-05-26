#ifndef ASCII_ART_H
#define ASCII_ART_H

// ASCII字符画风格枚举
typedef enum
{
    ASCII_STYLE_SIMPLE,   // 简单字符集 (6个字符)
    ASCII_STYLE_EXTENDED, // 扩展字符集 (13个字符)
    ASCII_STYLE_BLOCKS,   // 块状字符集 (ASCII兼容)
    ASCII_STYLE_DENSE,    // 密集字符集 (15个字符)
    ASCII_STYLE_CLASSIC   // 经典字符集 (完全ASCII兼容)
} ascii_style_t;

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

/**
 * @brief 将图像转换为指定风格的 ASCII 字符画并保存到文件中。
 * @param data 图像数据。
 * @param width 图像宽度。
 * @param height 图像高度。
 * @param channels 图像通道数。
 * @param output_file 输出ASCII字符画的文件路径。
 * @param scale_factor 缩放因子，用于调整输出字符画的大小，值越大输出越小。
 * @param style ASCII字符画风格。
 * @param gamma 伽马校正值，用于调整对比度 (0.5-2.0，默认0.8)。
 * @return 成功返回1，失败返回0。
 */
int image_to_ascii_styled(unsigned char *data,
                          int width,
                          int height,
                          int channels,
                          const char *output_file,
                          int scale_factor,
                          ascii_style_t style,
                          float gamma);

#endif
