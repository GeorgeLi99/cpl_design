// 定义 STB_IMAGE_IMPLEMENTATION 宏以包含 stb_image.h 的实现
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// 定义 STB_IMAGE_WRITE_IMPLEMENTATION 宏以包含 stb_image_write.h 的实现
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <stdio.h>  // 用于标准输入输出，如 printf, fprintf
#include <stdlib.h> // 用于标准库函数，如 exit
#include <string.h> // 用于字符串处理函数
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
        return 1;
    }

    // 输出目录，默认为当前目录
    const char *output_dir = (argc >= 3) ? argv[2] : "./";

    // 创建输出文件名
    char grayscale_output[256];
    char blur_output[256];
    char invert_output[256];
    char rotate_output[256];

    sprintf(grayscale_output, "%s/grayscale_output.jpg", output_dir);
    sprintf(blur_output, "%s/blur_output.jpg", output_dir);
    sprintf(invert_output, "%s/invert_output.jpg", output_dir);
    sprintf(rotate_output, "%s/rotate_output.jpg", output_dir);

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

    // 5. 生成ASCII字符画
    if (original_data) {
        image_to_ascii(original_data, width, height, channels, "output.txt", 5);
        // 释放图像数据
        stbi_image_free(original_data);
    }

    printf("All processing completed.\n");
    return 0;
}
