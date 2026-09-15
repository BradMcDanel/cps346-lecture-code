#include <cstdio>
#include <cstdlib>
#include <vector>

#include "image.h"
#include "simd.h"
#include "utils.h"

void contrast_scalar(const float *input, float *output, std::size_t n,
                     float scale, float bias) {
  for (std::size_t i = 0; i < n; i++) {
    float value = scale * input[i] + bias;
    if (value < 0.0f) {
      value = 0.0f;
    }
    if (value > 1.0f) {
      value = 1.0f;
    }
    output[i] = value;
  }
}

void contrast_simd(const float *input, float *output, std::size_t n,
                   float scale, float bias) {
  // TODO: replace the scalar version with SIMD intrinsics.
  contrast_scalar(input, output, n, scale, bias);
}

int main(int argc, char **argv) {
  if (argc < 3) {
    printf("usage: %s <input.bmp> <output.bmp> [scale] [bias]\n", argv[0]);
    return 1;
  }

  float scale = argc > 3 ? std::atof(argv[3]) : 1.5f;
  float bias = argc > 4 ? std::atof(argv[4]) : -0.25f;
  Image input;
  if (!load_bmp(argv[1], input)) {
    printf("Could not read %s.\n", argv[1]);
    return 1;
  }

  std::vector<float> expected(input.pixels.size());
  Image output = input;
  Timer timer;

  timer.start();
  contrast_scalar(input.pixels.data(), expected.data(), input.pixels.size(),
                  scale, bias);
  double scalar_seconds = timer.stop();

  timer.start();
  contrast_simd(input.pixels.data(), output.pixels.data(), input.pixels.size(),
                scale, bias);
  double simd_seconds = timer.stop();

  if (!save_bmp(argv[2], output)) {
    printf("Could not write %s.\n", argv[2]);
    return 1;
  }

  printf("%s   %d x %d\n", SIMD_NAME, input.width, input.height);
  printf("scalar: %.6f s   simd: %.6f s\n", scalar_seconds, simd_seconds);
  printf("max error: %.6g\n",
         max_error(expected.data(), output.pixels.data(), input.pixels.size()));
}
