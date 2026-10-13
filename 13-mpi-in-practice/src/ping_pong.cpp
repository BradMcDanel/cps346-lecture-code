// ping_pong.cpp
//
// Usage: mpirun -np 2 ./ping_pong

#include <cstdio>
#include <mpi.h>
#include <vector>

// One round trip is two one-way trips.
double one_way_seconds(char *buf, int bytes, int reps, int rank) {
  MPI_Barrier(MPI_COMM_WORLD);
  double start = MPI_Wtime();
  for (int r = 0; r < reps; r++) {
    // TODO: rank 0 sends buf to rank 1, then rank 1 sends it back.
  }
  return (MPI_Wtime() - start) / reps / 2;
}

int main(int argc, char **argv) {
  MPI_Init(&argc, &argv);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  std::vector<char> buf(16 << 20, 1);

  if (rank == 0)
    printf("%12s %14s %14s\n", "bytes", "time (us)", "GB/s");
  for (int bytes = 8; bytes <= (16 << 20); bytes *= 8) {
    int reps = bytes < 100000 ? 10000 : 100;
    double t = one_way_seconds(buf.data(), bytes, reps, rank);
    if (rank == 0)
      printf("%12d %14.2f %14.3f\n", bytes, t * 1e6, bytes / t / 1e9);
  }

  MPI_Finalize();
}
