# Shared memory and tiling

## Build

```sh
cd 16-matmul
cmake -S . -B build
cmake --build build
```

## Run

```sh
./build/dot
./build/dot 1024

./build/matmul 1024
./build/matmul_tiled 1024
```
