#define STB_IMAGE_IMPLEMENTATION
#include "../third_party/stb/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../third_party/stb/stb_image_write.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input_image> <output_image>\n", argv[0]);
        return 1;
    }
    int width, height, channels;
    unsigned char *data = stbi_load(argv[1], &width, &height, &channels, 0);
    if (data == NULL) {
        fprintf(stderr, "Error loading '%s': %s\n", argv[1], stbi_failure_reason());
        exit(1);
    }
    printf("Loaded '%s': %dx%d, %d channels\n", argv[1], width, height, channels);
    // 灰度化处理
    size_t pixel_count = (size_t)width * height;
    for (size_t i = 0; i < pixel_count; i++) {
        unsigned char *p = data + i * channels;
        unsigned char gray = (unsigned char)(0.299f * p[0] + 0.587f * p[1] + 0.114f * p[2]);
        p[0] = p[1] = p[2] = gray;
    }
    // 保存灰度图像
    if (!stbi_write_jpg(argv[2], width, height, channels, data, 90)) {
        fprintf(stderr, "Error writing '%s'\n", argv[2]);
        stbi_image_free(data);
        return 1;
    }
    printf("Saved grayscale image to '%s'\n", argv[2]);
    stbi_image_free(data);
    return 0;
}
