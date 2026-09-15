#ifndef SIMD_H
#define SIMD_H

// x86-64 (Intel/AMD):
// https://www.intel.com/content/www/us/en/docs/intrinsics-guide/index.html
//
// AArch64 (Apple Silicon):
// https://arm-software.github.io/acle/neon_intrinsics/advsimd.html

#if defined(__aarch64__) || defined(_M_ARM64)
#define SIMD_NEON
#include <arm_neon.h>
static const char *const SIMD_NAME = "Arm Neon";
#elif defined(__x86_64__) || defined(_M_X64)
#define SIMD_X86
#include <immintrin.h>
static const char *const SIMD_NAME = "x86 SSE";
#else
#error "This example requires x86-64 or AArch64."
#endif

#endif
