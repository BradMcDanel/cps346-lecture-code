# CUDA basics

Requires the CUDA toolkit (`nvcc`) and an NVIDIA GPU. Check both first:

```sh
nvidia-smi
nvcc --version
```

## Build

```sh
cd 15-cuda-basics
cmake -S . -B build
cmake --build build
```

## Part 1: who am I?

Before each run, predict what it will print.

```sh
./build/whoami
./build/whoami 3 4
./build/whoami 1 2048
```

## Part 2: add one

```sh
./build/add_one
./build/add_one 1 32
./build/add_one 1 2048
```

## Part 3: elementwise add

Implement `src/elementwise_add.cu` by following the steps in the file.
Use `add_one.cu` as a reference. Each element of the result should be 50.

```sh
cmake --build build
./build/elementwise_add
```

## Part 4: how big before the GPU wins?

Fill in the kernel and the number of blocks in `src/vector_add.cu`.
Run it until it prints `check: ok`. Then fill in the table.

```sh
cmake --build build
./build/vector_add 1000
```

| n          | cpu loop (ms) | copy to gpu (ms) | kernel (ms) | copy to cpu (ms) |
|------------|---------------|------------------|-------------|------------------|
| 1,000      |               |                  |             |                  |
| 100,000    |               |                  |             |                  |
| 1,000,000  |               |                  |             |                  |
| 10,000,000 |               |                  |             |                  |
| 50,000,000 |               |                  |             |                  |
