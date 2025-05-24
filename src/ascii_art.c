#include "ascii_art.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
int image_to_ascii(unsigned char *data, int width, int height, int channels, const char *output_file, int scale_factor)
{
    if (!data || width <= 0 || height <= 0 || channels <= 0 || !output_file || scale_factor <= 0) {
        fprintf(stderr, "Invalid parameters for image_to_ascii\n");
        return 0;
    }

    // ASCII字符集，从暗到亮排序
    const char *ascii_chars = " .:-=+*#%@";
    int ascii_chars_len = strlen(ascii_chars);

    // 计算缩放后的尺寸
    int scaled_width = width / scale_factor;
    int scaled_height = height / scale_factor;

    // 打开输出文件
    FILE *fp = fopen(output_file, "w");
    if (!fp) {
        fprintf(stderr, "Error opening output file '%s'\n", output_file);
        return 0;
    }

    // 写入ASCII字符画信息头
    fprintf(fp, "/* ASCII Art - Generated from image with %dx%d pixels */\n\n", width, height);

    // 遍历图像，按缩放比例生成ASCII字符
    for (int y = 0; y < height; y += scale_factor) {
        for (int x = 0; x < width; x += scale_factor) {
            // 计算当前块的平均亮度
            float brightness = 0.0f;
            int count = 0;

            // 计算当前块的像素平均值
            for (int by = 0; by < scale_factor && (y + by) < height; by++) {
                for (int bx = 0; bx < scale_factor && (x + bx) < width; bx++) {
                    int idx = ((y + by) * width + (x + bx)) * channels;
                    // 计算灰度值 - 使用加权RGB
                    float gray = 0;
                    if (channels >= 3) {
                        // 使用标准的RGB加权转换为灰度: 0.299R + 0.587G + 0.114B
                        gray = 0.299f * data[idx] + 0.587f * data[idx + 1] + 0.114f * data[idx + 2];
                    }
                    else {
                        // 如果是灰度图像，直接使用亮度值
                        gray = data[idx];
                    }
                    brightness += gray;
                    count++;
                }
            }

            // 计算平均亮度
            if (count > 0) {
                brightness /= count;
            }

            // 将亮度映射到ASCII字符范围 (0-255 -> 0-strlen(ascii_chars)-1)
            int char_idx = (int)((brightness / 255.0f) * (ascii_chars_len - 1) + 0.5f);

            // 确保索引在有效范围内
            if (char_idx < 0)
                char_idx = 0;
            if (char_idx >= ascii_chars_len)
                char_idx = ascii_chars_len - 1;

            // 写入对应的ASCII字符
            fprintf(fp, "%c", ascii_chars[char_idx]);
        }
        // 每行结束后换行
        fprintf(fp, "\n");
    }

    // 关闭文件
    fclose(fp);
    printf("Successfully saved ASCII art to '%s'\n", output_file);
    return 1;
}
