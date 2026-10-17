# GPU introduction

Requires the CUDA toolkit (`nvcc`) and an NVIDIA GPU to run the examples.

## Build and run

```sh
cd 14-gpu-intro
cmake -S . -B build
cmake --build build
./build/allocation
```

## Group work

Implement `src/elementwise_add.cu` by following the instructions in the file.
Feel free to work in groups. Reference `allocation.cu` for memory allocation and
transfers. The GPU function itself is the easy part!

Build again and run:

```sh
cmake --build build
./build/elementwise_add
```

For the provided inputs, each element of the result should be 50.
