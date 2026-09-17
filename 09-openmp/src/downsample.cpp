#include <cstdio>
#include <cstdlib>
#include <omp.h>

#include "image.h"
#include "utils.h"

Image downsample(const Image &input, int scale) {
  Image output;
  output.width = input.width / scale;
  output.height = input.height / scale;
  output.pixels.resize(std::size_t(output.width) * output.height * 3);

  // TODO: copy one pixel from each scale-by-scale input region.

  return output;
}

int main(int argc, char **argv) {
  if (argc < 3) {
    printf("usage: %s <input.bmp> <output.bmp> [scale]\n", argv[0]);
    return 1;
  }

  int scale = argc > 3 ? atoi(argv[3]) : 4;
  Image input;
  if (scale < 1 || !load_bmp(argv[1], input)) {
    printf("Could not read the input or use that scale.\n");
    return 1;
  }

  Timer timer;
  timer.start();
  Image output = downsample(input, scale);
  double seconds = timer.stop();

  if (!save_bmp(argv[2], output)) {
    printf("Could not write %s.\n", argv[2]);
    return 1;
  }

  printf("%d x %d -> %d x %d\n", input.width, input.height, output.width,
         output.height);
  printf("max threads: %d   time: %.6f s\n", omp_get_max_threads(), seconds);
}
