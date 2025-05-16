#include <stdio.h>
#include <stdlib.h>

#include "../third_party/stb/stb_image.h"

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <image_path>\n", argv[0]);
        return 1;
    }
    int width, height, channels;
    unsigned char *data = stbi_load("image.jpg", &width, &height, &channels, 0);
    if (data == NULL) {
        fprintf(stderr, "Error loading 'image.jpg': %s\n", stbi_failure_reason());
        exit(1);
    }
    printf("Loaded 'image.jpg': %dx%d, %d channels\n", width, height, channels);
    stbi_image_free(data);
    return 0;
}
