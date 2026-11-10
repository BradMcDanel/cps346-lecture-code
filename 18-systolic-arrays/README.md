# Systolic arrays

## Build

Linux and WSL only, once:

```sh
sudo apt install libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev libgl1-mesa-dev
```

Then:

```sh
cd 18-systolic-arrays
cmake -S . -B build
cmake --build build
```

## Run

The arguments are M, K and N: W is M x K, D is K x N and R is M x N. A last argument sets the skew (how many cycles each column of the array waits after the one to its left). The default is 1.

```sh
./build/viewer
./build/viewer 4 4 1
./build/viewer 4 4 4 0
./build/viewer 4 4 4 2
./build/check
./build/check 128 128 1024
```
