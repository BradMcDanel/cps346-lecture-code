## Install MPI

Linux / WSL:

```
sudo apt update
sudo apt install openmpi-bin libopenmpi-dev
```

macOS:

```
brew install open-mpi
```

## Check that it works

```
cd 12-mpi
cmake -S . -B build
cmake --build build
mpirun -np 4 ./build/hello
```

You should see four lines, `rank 0 of 4` through `rank 3 of 4`, in some order.
If `mpirun` says there are not enough slots, add `--oversubscribe`.
