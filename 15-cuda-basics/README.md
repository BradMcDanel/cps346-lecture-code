# CUDA basics

## Build

```sh
cd 15-cuda-basics
cmake -S . -B build
cmake --build build
```

## Run

```sh
./build/hello
./build/hello 3 4

./build/add_one
./build/add_one 1 32

./build/elementwise_add
```
