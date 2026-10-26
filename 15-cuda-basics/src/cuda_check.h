// cuda_check.h
#pragma once

#include <stdio.h>
#include <stdlib.h>

#define CUDA_CHECK(call)                                                    \
  do {                                                                      \
    cudaError_t err = (call);                                               \
    if (err != cudaSuccess) {                                               \
      fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__,                    \
              cudaGetErrorString(err));                                     \
      exit(1);                                                              \
    }                                                                       \
  } while (0)
