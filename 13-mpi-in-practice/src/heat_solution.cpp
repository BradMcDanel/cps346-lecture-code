#include <cstdio>
#include <mpi.h>
#include <vector>

#include "image.h"

const int N = 1200;
const int STEPS = 180000;
const int FRAME_EVERY = 300;

bool held(int i, double &t) {
  if (i == 0 || i == N - 1) {
    t = 0.0;
    return true;
  }
  if ((i >= 280 && i < 300) || (i >= 760 && i < 770)) {
    t = 1.0;
    return true;
  }
  return false;
}

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);
  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  int count = N / size;
  int first = rank * count;

  std::vector<double> cur(count + 2, 0.0), next(count + 2, 0.0);
  for (int k = 1; k <= count; k++) {
    int i = first + k - 1;
    double t;
    if (held(i, t))
      cur[k] = t;
    else if (i >= 980 && i < 1060)
      cur[k] = 1.0;
  }

  Image image;
  std::vector<double> rod(N);
  double start = MPI_Wtime();

  for (int step = 0; step < STEPS; step++) {
    if (step % FRAME_EVERY == 0) {
      if (rank != 0) {
        MPI_Send(&cur[1], count, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
      } else {
        for (int k = 0; k < count; k++)
          rod[k] = cur[k + 1];
        for (int r = 1; r < size; r++)
          MPI_Recv(&rod[r * count], count, MPI_DOUBLE, r, 1, MPI_COMM_WORLD,
                   MPI_STATUS_IGNORE);
        image.add_row(rod);
      }
    }

    if (rank > 0)
      MPI_Send(&cur[1], 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD);
    if (rank < size - 1)
      MPI_Send(&cur[count], 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD);
    if (rank > 0)
      MPI_Recv(&cur[0], 1, MPI_DOUBLE, rank - 1, 0, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);
    if (rank < size - 1)
      MPI_Recv(&cur[count + 1], 1, MPI_DOUBLE, rank + 1, 0, MPI_COMM_WORLD,
               MPI_STATUS_IGNORE);

    for (int k = 1; k <= count; k++) {
      double t;
      if (held(first + k - 1, t))
        next[k] = t;
      else
        next[k] = 0.25 * cur[k - 1] + 0.5 * cur[k] + 0.25 * cur[k + 1];
    }
    cur.swap(next);
  }

  if (rank == 0) {
    printf("ranks: %d   time: %.3f s\n", size, MPI_Wtime() - start);
    image.save("rod.bmp", size);
  }
  MPI_Finalize();
}
