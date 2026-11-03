// image.h
// Pixel (x, y) is stored as r, g, b bytes at index (y * w + x) * 3.
#pragma once

#include <stdio.h>
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

static unsigned char *load_image(const char *path, int *w, int *h) {
  int channels;
  unsigned char *img = stbi_load(path, w, h, &channels, 3);
  if (img == NULL) {
    printf("could not read %s (run from the 17-image-filters folder)\n", path);
    exit(1);
  }
  return img;
}

static void write_image(const char *path, const unsigned char *img, int w, int h) {
  stbi_write_png(path, w, h, 3, img, w * 3);
  printf("wrote %s\n", path);
}

static void check(const unsigned char *gpu, const unsigned char *cpu, int w, int h) {
  int wrong = 0;
  for (int i = 0; i < w * h; i++) {
    if (gpu[i * 3] != cpu[i * 3] || gpu[i * 3 + 1] != cpu[i * 3 + 1] ||
        gpu[i * 3 + 2] != cpu[i * 3 + 2]) {
      wrong++;
    }
  }
  if (wrong == 0) {
    printf("check: ok\n");
  } else {
    printf("check: %d of %d pixels wrong\n", wrong, w * h);
  }
}
