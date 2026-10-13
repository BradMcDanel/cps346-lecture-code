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

  // If I wanted to write a black row to the image, this is how.. but this isn't the right starting conditions!
  std::vector<double> rod(N, 0.0);
  image.add_row(rod);

  // TODO

  image.save("rod.bmp", 1);
  MPI_Finalize();
}
