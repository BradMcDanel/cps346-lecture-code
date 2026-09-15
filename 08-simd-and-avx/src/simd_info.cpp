#include <cstdio>

#include "simd.h"

int main() {
#if defined(SIMD_NEON)
  printf("Architecture: AArch64\n");
  printf("SIMD: Arm Neon\n");
  printf("Reference: https://arm-software.github.io/acle/neon_intrinsics/advsimd.html\n");
#else
  printf("Architecture: x86-64\n");
  printf("SIMD: SSE\n");
  printf("Reference: https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html\n");
#endif
}
