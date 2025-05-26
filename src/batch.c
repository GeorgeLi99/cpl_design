#include "batch.h"
#include "image.h"
#include "filters.h"
#include "rotate.h"
#include "ascii_art.h"
#include "edge.h"
#include "stb_image.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <ctype.h>
#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define mkdir(path, mode) _mkdir(path)
#else
#include <unistd.h>
#endif

/**
 * @brief 创建目录（如果不存在）
 * @param dir_path 目录路径
 * @return 成功返回1，失败返回0
 */
static int create_directory_if_not_exists(const char *dir_path)
{
    struct stat st = {0};
    if (stat(dir_path, &st) == -1) {
        // 目录不存在，创建它
#ifdef _WIN32
        if (_mkdir(dir_path) != 0) {
#else
        if (mkdir(dir_path, 0755) != 0) {
#endif
            fprintf(stderr, "Error creating directory: %s\n", dir_path);
            return 0;
        }
        printf("Created directory: %s\n", dir_path);
    }
    return 1;
}

/**
 * @brief 检查文件是否为支持的图像格式
 * @param filename 文件名
 * @return 是图像返回1，否则返回0
 */
static int is_image_file(const char *filename)
{
    const char *ext = strrchr(filename, '.');
    if (!ext)
        return 0;

    ext++; // 跳过点号

    // 转换为小写进行比较
    char ext_lower[10] = {0};
    int i;
    for (i = 0; ext[i] && i < 9; i++) {
        ext_lower[i] = tolower(ext[i]);
    }

    return (strcmp(ext_lower, "jpg") == 0 || strcmp(ext_lower, "jpeg") == 0 || strcmp(ext_lower, "png") == 0 ||
            strcmp(ext_lower, "bmp") == 0);
}

/**
 * @brief 从文件名中提取基本名称（不含路径和扩展名）
 * @param filename 完整文件名
 * @param basename 输出的基本名称缓冲区
 * @param size 缓冲区大小
 */
static void extract_basename(const char *filename, char *basename, size_t size)
{
    const char *last_slash = strrchr(filename, '/');
#ifdef _WIN32
    const char *last_backslash = strrchr(filename, '\\');
    if (last_backslash && (!last_slash || last_backslash > last_slash)) {
        last_slash = last_backslash;
    }
#endif

    const char *name_start = last_slash ? last_slash + 1 : filename;
    const char *ext = strrchr(name_start, '.');

    size_t name_len = ext ? (size_t)(ext - name_start) : strlen(name_start);
    if (name_len >= size)
        name_len = size - 1;

    strncpy(basename, name_start, name_len);
    basename[name_len] = '\0';
}

/**
 * @brief 执行批量图像处理。
 */
void batch_process()
{
    // 输入和输出目录
    const char *input_dir = "./batch_input";
    const char *output_dir = "./batch_output";

    // 确保输出目录存在
    if (!create_directory_if_not_exists(output_dir)) {
        fprintf(stderr, "Failed to create output directory: %s\n", output_dir);
        return;
    }

    // 创建输出子目录
    char grayscale_dir[256], blur_dir[256], invert_dir[256], rotate_dir[256], ascii_dir[256], edge_dir[256];
#ifdef _WIN32
    sprintf(grayscale_dir, "%s\\grayscale", output_dir);
    sprintf(blur_dir, "%s\\blur", output_dir);
    sprintf(invert_dir, "%s\\invert", output_dir);
    sprintf(rotate_dir, "%s\\rotate", output_dir);
    sprintf(ascii_dir, "%s\\ascii", output_dir);
    sprintf(edge_dir, "%s\\edge", output_dir);
#else
    sprintf(grayscale_dir, "%s/grayscale", output_dir);
    sprintf(blur_dir, "%s/blur", output_dir);
    sprintf(invert_dir, "%s/invert", output_dir);
    sprintf(rotate_dir, "%s/rotate", output_dir);
    sprintf(ascii_dir, "%s/ascii", output_dir);
    sprintf(edge_dir, "%s/edge", output_dir);
#endif

    create_directory_if_not_exists(grayscale_dir);
    create_directory_if_not_exists(blur_dir);
    create_directory_if_not_exists(invert_dir);
    create_directory_if_not_exists(rotate_dir);
    create_directory_if_not_exists(ascii_dir);
    create_directory_if_not_exists(edge_dir);

    // 打开输入目录
    DIR *dir = opendir(input_dir);
    if (!dir) {
        fprintf(stderr, "Error opening input directory: %s\n", input_dir);
        return;
    }

    printf("Starting batch processing of images in %s\n", input_dir);

    // 读取目录中的每个文件
    struct dirent *entry;
    int processed_count = 0;

    while ((entry = readdir(dir)) != NULL) {
        // 跳过"."和".."
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // 检查是否为图像文件
        if (!is_image_file(entry->d_name)) {
            continue;
        }

        // 构建完整的输入文件路径
        char input_path[512];
#ifdef _WIN32
        sprintf(input_path, "%s\\%s", input_dir, entry->d_name);
#else
        sprintf(input_path, "%s/%s", input_dir, entry->d_name);
#endif

        // 提取基本文件名（不含扩展名）
        char basename[256];
        extract_basename(entry->d_name, basename, sizeof(basename));

        // 构建输出文件路径
        char grayscale_output[512], blur_output[512], invert_output[512], rotate_output[512], ascii_output[512],
            edge_output[512];
#ifdef _WIN32
        sprintf(grayscale_output, "%s\\%s_grayscale.jpg", grayscale_dir, basename);
        sprintf(blur_output, "%s\\%s_blur.jpg", blur_dir, basename);
        sprintf(invert_output, "%s\\%s_invert.jpg", invert_dir, basename);
        sprintf(rotate_output, "%s\\%s_rotate.jpg", rotate_dir, basename);
        sprintf(ascii_output, "%s\\%s_ascii.txt", ascii_dir, basename);
        sprintf(edge_output, "%s\\%s_edge.jpg", edge_dir, basename);
#else
        sprintf(grayscale_output, "%s/%s_grayscale.jpg", grayscale_dir, basename);
        sprintf(blur_output, "%s/%s_blur.jpg", blur_dir, basename);
        sprintf(invert_output, "%s/%s_invert.jpg", invert_dir, basename);
        sprintf(rotate_output, "%s/%s_rotate.jpg", rotate_dir, basename);
        sprintf(ascii_output, "%s/%s_ascii.txt", ascii_dir, basename);
        sprintf(edge_output, "%s/%s_edge.jpg", edge_dir, basename);
#endif

        printf("Processing file: %s\n", input_path);

        // 加载图像
        int width, height, channels;
        unsigned char *image_data = load_image(input_path, &width, &height, &channels);
        if (!image_data) {
            fprintf(stderr, "Failed to load image: %s\n", input_path);
            continue;
        }

        // 1. 灰度处理
        unsigned char *grayscale_data = (unsigned char *)malloc(width * height * channels);
        if (grayscale_data) {
            memcpy(grayscale_data, image_data, width * height * channels);
            grayscale(grayscale_data, width, height, channels);
            save_image(grayscale_output, grayscale_data, width, height, channels, 100);
            free(grayscale_data);
        }

        // 2. 模糊处理
        unsigned char *blur_data = (unsigned char *)malloc(width * height * channels);
        if (blur_data) {
            memcpy(blur_data, image_data, width * height * channels);
            blur(blur_data, width, height, channels, 5); // 使用半径5的模糊
            save_image(blur_output, blur_data, width, height, channels, 100);
            free(blur_data);
        }

        // 3. 反色处理
        unsigned char *invert_data = (unsigned char *)malloc(width * height * channels);
        if (invert_data) {
            memcpy(invert_data, image_data, width * height * channels);
            invert(invert_data, width, height, channels);
            save_image(invert_output, invert_data, width, height, channels, 100);
            free(invert_data);
        }

        // 4. 旋转处理
        unsigned char *rotate_data = (unsigned char *)malloc(width * height * channels);
        if (rotate_data) {
            memcpy(rotate_data, image_data, width * height * channels);
            rotate_image(rotate_data, width, height, channels);
            save_image(rotate_output, rotate_data, width, height, channels, 100);
            free(rotate_data);
        }

        // 5. ASCII字符画
        image_to_ascii_styled(image_data, width, height, channels, ascii_output, 5, ASCII_STYLE_EXTENDED, 0.6f);

        // 6. 边缘检测（如果实现了）
        // TODO: 实现边缘检测函数

        // 释放图像数据
        stbi_image_free(image_data);

        processed_count++;
        printf("Completed processing: %s\n", entry->d_name);
    }

    closedir(dir);

    printf("Batch processing complete. Processed %d images.\n", processed_count);
    printf("Results saved to %s\n", output_dir);
}
