#include <stdio.h>
#include <stdlib.h>

__global__ void add(int *a, int *b, int *c, int n) {
  // TODO: add `a` and `b` and store the result in `c`
  //       use `n` to determine the size of the arrays (number of elements)
}

int main() {
  int n = 50;
  int *a = (int *)malloc(n * sizeof(int));
  int *b = (int *)malloc(n * sizeof(int));
  int *c = (int *)malloc(n * sizeof(int));

  // initialize `a` and `b` with some values
  for (int i = 0; i < n; i++) {
    a[i] = i;
    b[i] = n - i;
  }

  // (1) allocate device memory for `a`, `b`, and `c`
  // (2) copy `a` and `b` to the device
  // (3) launch the kernel
  //     Hint: how many blocks and threads do you need?
  // (4) copy `c` back to the host
  // (5) print the result
  // (6) free device memory
  // (7) free host memory
}
