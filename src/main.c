#include <stdio.h>
#include <stdlib.h>

#include "stb_image.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s <image_path>\n", argv[0]);
        return 1;
    }
    int width, height, channels;
    unsigned char *data = stbi_load(argv[1], &width, &height, &channels, 0);
    if (data == NULL)
    {
        fprintf(stderr, "Error loading '%s': %s\n", argv[1], stbi_failure_reason());
        exit(1);
    }
    printf("Loaded '%s': %dx%d, %d channels\n", argv[1], width, height, channels);
    stbi_image_free(data);
    return 0;
}
