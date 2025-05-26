#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "image.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief 从指定路径加载图像。
 * @param path 图像文件的路径。
 * @param width 指向存储图像宽度的变量的指针。
 * @param height 指向存储图像高度的变量的指针。
 * @param channels 指向存储图像通道数的变量的指针。
 * @return 成功返回图像数据指针，失败返回NULL。
 */
unsigned char *load_image(const char *path, int *width, int *height, int *channels)
{
    if (!path || !width || !height || !channels) {
        fprintf(stderr, "Invalid parameters for load_image\n");
        return NULL;
    }

    // 使用 stb_image 加载图像
    unsigned char *data = stbi_load(path, width, height, channels, 0);
    if (!data) {
        fprintf(stderr, "Error loading image '%s': %s\n", path, stbi_failure_reason());
        return NULL;
    }

    printf("Successfully loaded image '%s' (%dx%d, %d channels)\n", path, *width, *height, *channels);
    return data;
}

/**
 * @brief 将图像保存到指定路径。
 * @param path 保存图像文件的路径。
 * @param data 图像数据。
 * @param width 图像宽度。
 * @param height 图像高度。
 * @param channels 图像通道数。
 * @param quality JPEG质量参数 (1-100)，仅对JPEG格式有效。
 * @return 成功返回1，失败返回0。
 */
int save_image(const char *path, unsigned char *data, int width, int height, int channels, int quality)
{
    if (!path || !data || width <= 0 || height <= 0 || channels <= 0) {
        fprintf(stderr, "Invalid parameters for save_image\n");
        return 0;
    }

    // 根据文件扩展名确定保存格式
    const char *ext = strrchr(path, '.');
    if (!ext) {
        fprintf(stderr, "No file extension found in path '%s'\n", path);
        return 0;
    }

    int result = 0;
    if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) {
        // 保存为JPEG格式
        result = stbi_write_jpg(path, width, height, channels, data, quality);
    }
    else if (strcmp(ext, ".png") == 0) {
        // 保存为PNG格式
        result = stbi_write_png(path, width, height, channels, data, width * channels);
    }
    else if (strcmp(ext, ".bmp") == 0) {
        // 保存为BMP格式
        result = stbi_write_bmp(path, width, height, channels, data);
    }
    else if (strcmp(ext, ".tga") == 0) {
        // 保存为TGA格式
        result = stbi_write_tga(path, width, height, channels, data);
    }
    else {
        fprintf(stderr, "Unsupported file format: %s\n", ext);
        return 0;
    }

    if (!result) {
        fprintf(stderr, "Error saving image to '%s'\n", path);
        return 0;
    }

    printf("Successfully saved image to '%s'\n", path);
    return 1;
}