// heat.cpp
//
// Usage: mpirun -np <ranks> ./heat

#include <cstdio>
#include <mpi.h>
#include <vector>

#include "image.h"

const int N = 1200;
const int STEPS = 180000;
const int FRAME_EVERY = 300;

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);

  Image image;
  // TODO

  image.save("rod.bmp", 1);
  MPI_Finalize();
}
