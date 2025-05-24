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
    // 检查命令行参数是否足够（程序名 + 输入文件 + 输出文件）
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input_image> <output_image>\n", argv[0]);
        return 1; // 参数不足，返回错误码1
    }

    int width, height, channels; // 存储图像的宽度、高度和通道数
    // 加载图像，argv[1] 是输入图像的路径
    // 0 表示自动检测通道数，也可以强制指定通道数（如3为RGB，4为RGBA）
    unsigned char *data = stbi_load(argv[1], &width, &height, &channels, 0);
    // 检查图像是否加载成功
    if (data == NULL) {
        fprintf(stderr, "Error loading '%s': %s\n", argv[1], stbi_failure_reason());
        exit(1); // 加载失败，打印错误信息并退出
    }
    printf("Loaded '%s': %dx%d, %d channels\n", argv[1], width, height, channels);

    // 调用 grayscale 函数进行灰度化处理
    // grayscale(data, width, height, channels);
    // printf("Image grayscaled.\n");

    // 添加高斯模糊处理，使用半径为5的模糊效果
    int blur_radius = 5;
    blur(data, width, height, channels, blur_radius);
    printf("Applied Gaussian blur with radius %d.\n", blur_radius);

    // 保存处理后的图像
    // argv[2] 是输出图像的路径
    // 100 是JPEG图像的质量参数 (1-100, 越高图像质量越好，文件越大)
    if (!stbi_write_jpg(argv[2], width, height, channels, data, 100)) {
        fprintf(stderr, "Error writing '%s'\n", argv[2]);
        stbi_image_free(data); // 释放之前加载的图像数据
        return 1;              // 保存失败，返回错误码1
    }
    printf("Saved blurred image to '%s'\n", argv[2]);

    // 释放stb_image加载的图像数据，防止内存泄漏
    stbi_image_free(data);
    return 0; // 程序成功执行完毕
}
