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

    // 高对比度ASCII字符集，从暗到亮，确保明显的视觉差异
    const char *ascii_chars = " .-+*#@";
    int ascii_chars_len = strlen(ascii_chars);

    // 字符高宽比补偿，通常字符高度约是宽度的两倍
    // 调整此值可以改变输出字符画的垂直拉伸/压缩程度
    float aspect_ratio_correction = 2.0f;

    // 水平方向上，每个输出字符对应原始图像中的像素数
    int h_sample_step = scale_factor;
    // 垂直方向上，每个输出字符对应原始图像中的像素数（考虑高宽比补偿）
    int v_sample_step = (int)(scale_factor * aspect_ratio_correction);

    // 确保采样步长至少为1，防止除零或无效循环
    if (h_sample_step <= 0)
        h_sample_step = 1;
    if (v_sample_step <= 0)
        v_sample_step = 1;

    // 计算最终ASCII字符画的宽度（字符数）
    // 使用 (numerator + denominator - 1) / denominator 实现向上取整
    int ascii_art_width = (width + h_sample_step - 1) / h_sample_step;
    // 计算最终ASCII字符画的高度（字符数）
    int ascii_art_height = (height + v_sample_step - 1) / v_sample_step;

    FILE *fp = fopen(output_file, "w");
    if (!fp) {
        fprintf(stderr, "Error opening output file '%s'\n", output_file);
        return 0;
    }

    // 在输出文件中写入一些元信息
    fprintf(fp, "ASCII Art - Original Image: %dx%d pixels\n", width, height);
    fprintf(fp, "Output Dimensions: %d chars wide x %d chars high\n", ascii_art_width, ascii_art_height);
    fprintf(fp, "Sampling Step: Horizontal=%d pixels/char, Vertical=%d pixels/char\n", h_sample_step, v_sample_step);
    fprintf(fp, "Scale Factor: %d, Aspect Ratio Correction: %.1f\n\n", scale_factor, aspect_ratio_correction);

    // 遍历ASCII字符画的每一个字符位置（行优先）
    for (int char_y = 0; char_y < ascii_art_height; ++char_y) {
        for (int char_x = 0; char_x < ascii_art_width; ++char_x) {
            // 计算当前字符对应的原始图像区域的左上角坐标
            int original_region_x_start = char_x * h_sample_step;
            int original_region_y_start = char_y * v_sample_step;

            float sum_of_brightness_values = 0.0f;
            int num_pixels_in_current_block = 0;

            // 遍历当前字符对应的原始图像块内的所有像素
            for (int dy_in_block = 0; dy_in_block < v_sample_step; ++dy_in_block) {
                int current_original_pixel_y = original_region_y_start + dy_in_block;

                // 检查是否超出图像的垂直边界
                if (current_original_pixel_y >= height)
                    break;

                for (int dx_in_block = 0; dx_in_block < h_sample_step; ++dx_in_block) {
                    int current_original_pixel_x = original_region_x_start + dx_in_block;

                    // 检查是否超出图像的水平边界
                    if (current_original_pixel_x >= width)
                        break;

                    // 计算像素在data数组中的索引
                    int pixel_data_index = (current_original_pixel_y * width + current_original_pixel_x) * channels;
                    float pixel_gray_value = 0.0f;

                    if (channels >= 3) {                                         // 处理彩色图像 (RGB, RGBA)
                        pixel_gray_value = 0.299f * data[pixel_data_index] +     // Red
                                           0.587f * data[pixel_data_index + 1] + // Green
                                           0.114f * data[pixel_data_index + 2];  // Blue
                    }
                    else { // 处理灰度图像
                        pixel_gray_value = data[pixel_data_index];
                    }
                    sum_of_brightness_values += pixel_gray_value;
                    num_pixels_in_current_block++;
                }
            }

            float average_brightness_for_block = 0.0f;
            if (num_pixels_in_current_block > 0) {
                average_brightness_for_block = sum_of_brightness_values / num_pixels_in_current_block;
            }

            // 高对比度增强：结合伽马校正和对比度拉伸
            float gamma = 0.6f; // 更强的对比度增强
            float normalized_brightness = average_brightness_for_block / 255.0f;

            // 对比度拉伸：将中间值推向极端
            float contrast_enhanced = (normalized_brightness - 0.5f) * 1.5f + 0.5f;
            if (contrast_enhanced < 0.0f)
                contrast_enhanced = 0.0f;
            if (contrast_enhanced > 1.0f)
                contrast_enhanced = 1.0f;

            // 伽马校正进一步增强对比度
            float enhanced_brightness = powf(contrast_enhanced, gamma);

            // 将增强后的亮度映射到ASCII字符集中的一个字符
            int char_map_index = (int)(enhanced_brightness * (ascii_chars_len - 1) + 0.5f);

            // 边界检查，确保索引在有效范围内
            if (char_map_index < 0)
                char_map_index = 0;
            if (char_map_index >= ascii_chars_len)
                char_map_index = ascii_chars_len - 1;

            fprintf(fp, "%c", ascii_chars[char_map_index]);
        }
        fprintf(fp, "\n"); // 每行结束后换行
    }

    fclose(fp);
    printf("Successfully saved ASCII art to '%s'\n", output_file);
    printf("ASCII art dimensions: %d x %d characters\n", ascii_art_width, ascii_art_height);
    return 1;
}

/**
 * @brief 获取指定风格的ASCII字符集
 * @param style ASCII字符画风格
 * @return 字符集字符串
 */
static const char *get_ascii_charset(ascii_style_t style)
{
    switch (style) {
    case ASCII_STYLE_SIMPLE:
        // 高对比度简单字符集：每个字符都有明显的密度差异
        return " .-+*#@";

    case ASCII_STYLE_EXTENDED:
        // 高对比度扩展字符集：精心选择的字符，确保明显的视觉密度差异
        return " .,:;!+*oxO0#@";

    case ASCII_STYLE_BLOCKS:
        // 块状字符集：使用ASCII兼容字符避免乱码问题
        return " .:=#";

    case ASCII_STYLE_DENSE:
        // 密集高对比度字符集：每个字符都有明显的视觉密度差异
        return " .,;:!+*oxO08#@";

    case ASCII_STYLE_CLASSIC:
        // 经典ASCII字符集：完全兼容所有终端和编码
        return " .-:=+*%#@";

    default:
        return " .-+*#@";
    }
}

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
                          float gamma)
{
    if (!data || width <= 0 || height <= 0 || channels <= 0 || !output_file || scale_factor <= 0) {
        fprintf(stderr, "Invalid parameters for image_to_ascii_styled\n");
        return 0;
    }

    // 限制伽马值在合理范围内
    if (gamma < 0.1f)
        gamma = 0.1f;
    if (gamma > 3.0f)
        gamma = 3.0f;

    const char *ascii_chars = get_ascii_charset(style);
    int ascii_chars_len = strlen(ascii_chars);

    // 字符高宽比补偿
    float aspect_ratio_correction = 2.0f;

    int h_sample_step = scale_factor;
    int v_sample_step = (int)(scale_factor * aspect_ratio_correction);

    if (h_sample_step <= 0)
        h_sample_step = 1;
    if (v_sample_step <= 0)
        v_sample_step = 1;

    int ascii_art_width = (width + h_sample_step - 1) / h_sample_step;
    int ascii_art_height = (height + v_sample_step - 1) / v_sample_step;

    FILE *fp = fopen(output_file, "w");
    if (!fp) {
        fprintf(stderr, "Error opening output file '%s'\n", output_file);
        return 0;
    }

    // 写入文件头信息
    const char *style_names[] = {"Simple", "Extended", "Blocks", "Dense", "Classic"};
    fprintf(fp, "ASCII Art - Original Image: %dx%d pixels\n", width, height);
    fprintf(fp, "Output Dimensions: %d chars wide x %d chars high\n", ascii_art_width, ascii_art_height);
    fprintf(fp, "Style: %s (%d characters), Gamma: %.2f\n", style_names[style], ascii_chars_len, gamma);
    fprintf(fp, "Sampling: H=%d, V=%d pixels/char\n\n", h_sample_step, v_sample_step);

    // 生成ASCII字符画
    for (int char_y = 0; char_y < ascii_art_height; ++char_y) {
        for (int char_x = 0; char_x < ascii_art_width; ++char_x) {
            int original_region_x_start = char_x * h_sample_step;
            int original_region_y_start = char_y * v_sample_step;

            float sum_of_brightness_values = 0.0f;
            int num_pixels_in_current_block = 0;

            // 采样像素块
            for (int dy_in_block = 0; dy_in_block < v_sample_step; ++dy_in_block) {
                int current_original_pixel_y = original_region_y_start + dy_in_block;
                if (current_original_pixel_y >= height)
                    break;

                for (int dx_in_block = 0; dx_in_block < h_sample_step; ++dx_in_block) {
                    int current_original_pixel_x = original_region_x_start + dx_in_block;
                    if (current_original_pixel_x >= width)
                        break;

                    int pixel_data_index = (current_original_pixel_y * width + current_original_pixel_x) * channels;
                    float pixel_gray_value = 0.0f;

                    if (channels >= 3) {
                        pixel_gray_value = 0.299f * data[pixel_data_index] +     // Red
                                           0.587f * data[pixel_data_index + 1] + // Green
                                           0.114f * data[pixel_data_index + 2];  // Blue
                    }
                    else {
                        pixel_gray_value = data[pixel_data_index];
                    }
                    sum_of_brightness_values += pixel_gray_value;
                    num_pixels_in_current_block++;
                }
            }

            float average_brightness_for_block = 0.0f;
            if (num_pixels_in_current_block > 0) {
                average_brightness_for_block = sum_of_brightness_values / num_pixels_in_current_block;
            }

            // 伽马校正增强对比度
            float normalized_brightness = average_brightness_for_block / 255.0f;
            float enhanced_brightness = powf(normalized_brightness, gamma);

            // 映射到字符
            int char_map_index = (int)(enhanced_brightness * (ascii_chars_len - 1) + 0.5f);
            if (char_map_index < 0)
                char_map_index = 0;
            if (char_map_index >= ascii_chars_len)
                char_map_index = ascii_chars_len - 1;

            fprintf(fp, "%c", ascii_chars[char_map_index]);
        }
        fprintf(fp, "\n");
    }

    fclose(fp);
    printf("Successfully saved styled ASCII art to '%s'\n", output_file);
    printf("Style: %s, Dimensions: %d x %d characters, Gamma: %.2f\n",
           style_names[style],
           ascii_art_width,
           ascii_art_height,
           gamma);
    return 1;
}