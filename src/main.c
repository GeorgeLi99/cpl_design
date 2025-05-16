#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <pwd.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "stb_image.h"

int main(int argc, char* argv[]) {
  int width, height, channels;
  unsigned char* data = stbi_load("image.jpg", &width, &height, &channels, 0);
  if (data == NULL) {
    printf("Error in loading the image\n");
    exit(1);
  }
  printf("width: %d, height: %d, channels: %d\n", width, height, channels);
  stbi_image_free(data);
}
