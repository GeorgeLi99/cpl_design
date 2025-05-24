// 定义 STB_IMAGE_IMPLEMENTATION 宏以包含 stb_image.h 的实现
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
// 定义 STB_IMAGE_WRITE_IMPLEMENTATION 宏以包含 stb_image_write.h 的实现
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <stdio.h>  // 用于标准输入输出，如 printf, fprintf
#include <stdlib.h> // 用于标准库函数，如 exit
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
    // 加载原始图像
    unsigned char *original_data = stbi_load(argv[1], &width, &height, &channels, 0);
    if (original_data == NULL) {
        fprintf(stderr, "Error loading '%s': %s\n", argv[1], stbi_failure_reason());
        return 1;
    }
    printf("Loaded '%s': %dx%d, %d channels\n", argv[1], width, height, channels);

    // 1. 灰度处理
    unsigned char *grayscale_data = (unsigned char *)malloc(width * height * channels);
    if (grayscale_data) {
        memcpy(grayscale_data, original_data, width * height * channels);
        grayscale(grayscale_data, width, height, channels);
        printf("Applied grayscale filter.\n");

        if (stbi_write_jpg(grayscale_output, width, height, channels, grayscale_data, 100)) {
            printf("Saved grayscale image to '%s'\n", grayscale_output);
        }
        else {
            fprintf(stderr, "Error writing '%s'\n", grayscale_output);
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

        if (stbi_write_jpg(blur_output, width, height, channels, blur_data, 100)) {
            printf("Saved blurred image to '%s'\n", blur_output);
        }
        else {
            fprintf(stderr, "Error writing '%s'\n", blur_output);
        }
        free(blur_data);
    }

    // 3. 反色处理
    unsigned char *invert_data = (unsigned char *)malloc(width * height * channels);
    if (invert_data) {
        memcpy(invert_data, original_data, width * height * channels);
        invert(invert_data, width, height, channels);
        printf("Applied invert filter.\n");

        if (stbi_write_jpg(invert_output, width, height, channels, invert_data, 100)) {
            printf("Saved inverted image to '%s'\n", invert_output);
        }
        else {
            fprintf(stderr, "Error writing '%s'\n", invert_output);
        }
        free(invert_data);
    }

    // 4. 旋转处理
    unsigned char *rotate_data = (unsigned char *)malloc(width * height * channels);
    if (rotate_data) {
        memcpy(rotate_data, original_data, width * height * channels);
        rotate_image(rotate_data, width, height, channels);
        printf("Applied rotation.\n");

        if (stbi_write_jpg(rotate_output, width, height, channels, rotate_data, 100)) {
            printf("Saved rotated image to '%s'\n", rotate_output);
        }
        else {
            fprintf(stderr, "Error writing '%s'\n", rotate_output);
        }
        free(rotate_data);
    }

    // 释放原始图像数据
    stbi_image_free(original_data);
    printf("All processing completed.\n");
    return 0;
}
