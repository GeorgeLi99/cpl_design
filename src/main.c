#include <stdio.h>  // 用于标准输入输出，如 printf, fprintf
#include <stdlib.h> // 用于标准库函数，如 exit
#include <string.h> // 用于字符串处理函数
#include "stb_image.h"
#include "stb_image_write.h"
#include "image.h"
#include "filters.h"
#include "ascii_art.h"
#include "edge.h"
#include "rotate.h"
#include "batch.h"

/**
 * @brief 主函数，程序入口点。
 * @param argc 命令行参数数量。
 * @param argv 命令行参数数组。
 * @return 成功返回0，失败返回非0。
 */
int main(int argc, char *argv[])
{
    // 检查命令行参数
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <input_image> [output_dir]\n", argv[0]);
        fprintf(stderr, "       %s --batch    (批量处理batch_input目录中的所有图像)\n", argv[0]);
        return 1;
    }

    // 检查是否是批处理模式
    if (strcmp(argv[1], "--batch") == 0) {
        printf("Starting batch processing mode...\n");
        batch_process();
        return 0;
    }

    // 输出目录，默认为当前目录
    const char *output_dir = (argc >= 3) ? argv[2] : "./";

    // 创建输出文件名
    char grayscale_output[256];
    char blur_output[256];
    char invert_output[256];
    char rotate_output[256];
    char ascii_output[256];
    char ascii_output_simple[256];
    char ascii_output_extended[256];
    char ascii_output_blocks[256];
    char ascii_output_dense[256];
    char ascii_output_high_contrast[256];
    char ascii_output_classic[256];

    sprintf(grayscale_output, "%s/grayscale_output.jpg", output_dir);
    sprintf(blur_output, "%s/blur_output.jpg", output_dir);
    sprintf(invert_output, "%s/invert_output.jpg", output_dir);
    sprintf(rotate_output, "%s/rotate_output.jpg", output_dir);
    sprintf(ascii_output, "%s/ascii_output.txt", output_dir);
    sprintf(ascii_output_simple, "%s/ascii_output_simple.txt", output_dir);
    sprintf(ascii_output_extended, "%s/ascii_output_extended.txt", output_dir);
    sprintf(ascii_output_blocks, "%s/ascii_output_blocks.txt", output_dir);
    sprintf(ascii_output_dense, "%s/ascii_output_dense.txt", output_dir);
    sprintf(ascii_output_high_contrast, "%s/ascii_output_high_contrast.txt", output_dir);
    sprintf(ascii_output_classic, "%s/ascii_output_classic.txt", output_dir);

    int width, height, channels;
    // 使用封装的 load_image 函数加载原始图像
    unsigned char *original_data = load_image(argv[1], &width, &height, &channels);
    if (original_data == NULL) {
        // load_image 已经输出了错误信息，直接返回错误码
        return 1;
    }

    // 1. 灰度处理
    unsigned char *grayscale_data = (unsigned char *)malloc(width * height * channels);
    if (grayscale_data) {
        memcpy(grayscale_data, original_data, width * height * channels);
        grayscale(grayscale_data, width, height, channels);
        printf("Applied grayscale filter.\n");

        // 使用封装的 save_image 函数保存图像
        if (save_image(grayscale_output, grayscale_data, width, height, channels, 100)) {
            printf("Saved grayscale image to '%s'\n", grayscale_output);
        }
        free(grayscale_data);
    }

    // 2. 高斯模糊处理
    unsigned char *blur_data = (unsigned char *)malloc(width * height * channels);
    if (blur_data) {
        memcpy(blur_data, original_data, width * height * channels);
        int blur_radius = 10; // 可以调整模糊半径
        blur(blur_data, width, height, channels, blur_radius);
        printf("Applied Gaussian blur with radius %d.\n", blur_radius);

        // 使用封装的 save_image 函数保存图像
        if (save_image(blur_output, blur_data, width, height, channels, 100)) {
            printf("Saved blurred image to '%s'\n", blur_output);
        }
        free(blur_data);
    }

    // 3. 反色处理
    unsigned char *invert_data = (unsigned char *)malloc(width * height * channels);
    if (invert_data) {
        memcpy(invert_data, original_data, width * height * channels);
        invert(invert_data, width, height, channels);
        printf("Applied invert filter.\n");

        // 使用封装的 save_image 函数保存图像
        if (save_image(invert_output, invert_data, width, height, channels, 100)) {
            printf("Saved inverted image to '%s'\n", invert_output);
        }
        free(invert_data);
    }

    // 4. 旋转处理
    unsigned char *rotate_data = (unsigned char *)malloc(width * height * channels);
    if (rotate_data) {
        memcpy(rotate_data, original_data, width * height * channels);
        rotate_image(rotate_data, width, height, channels);
        printf("Applied rotation.\n");

        // 使用封装的 save_image 函数保存图像
        if (save_image(rotate_output, rotate_data, width, height, channels, 100)) {
            printf("Saved rotated image to '%s'\n", rotate_output);
        }
        free(rotate_data);
    }

    // 5. 生成ASCII字符画 - 多种风格
    if (original_data) {
        // 生成简单风格的ASCII字符画
        image_to_ascii(original_data, width, height, channels, ascii_output_simple, 5);

        // 生成扩展风格的ASCII字符画（更多字符，高对比度）
        image_to_ascii_styled(
            original_data, width, height, channels, ascii_output_extended, 5, ASCII_STYLE_EXTENDED, 0.6f);

        // 生成块状风格的ASCII字符画（最佳对比度）
        image_to_ascii_styled(original_data, width, height, channels, ascii_output_blocks, 8, ASCII_STYLE_BLOCKS, 0.8f);

        // 生成密集风格的ASCII字符画（高对比度）
        image_to_ascii_styled(original_data, width, height, channels, ascii_output_dense, 6, ASCII_STYLE_DENSE, 0.5f);

        // 生成超高对比度版本
        image_to_ascii_styled(
            original_data, width, height, channels, ascii_output_high_contrast, 4, ASCII_STYLE_EXTENDED, 0.4f);

        // 生成经典兼容版本（完全ASCII兼容，无乱码）
        image_to_ascii_styled(
            original_data, width, height, channels, ascii_output_classic, 6, ASCII_STYLE_CLASSIC, 0.7f);

        printf("Generated ASCII art in multiple high-contrast styles:\n");
        printf("  - ascii_output_simple.txt (6-character high-contrast set)\n");
        printf("  - ascii_output_extended.txt (13-character extended set, gamma=0.6)\n");
        printf("  - ascii_output_blocks.txt (ASCII block characters, gamma=0.8, no Unicode)\n");
        printf("  - ascii_output_dense.txt (15-character dense set, gamma=0.5)\n");
        printf("  - ascii_output_high_contrast.txt (ultra high contrast, gamma=0.4)\n");
        printf("  - ascii_output_classic.txt (classic 9-character set, gamma=0.7, fully compatible)\n");

        // 释放图像数据
        stbi_image_free(original_data);
    }

    printf("All processing completed.\n");
    return 0;
}
