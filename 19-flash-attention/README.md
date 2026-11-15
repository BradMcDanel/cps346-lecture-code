# FlashAttention

## Build

```sh
cd 19-flash-attention
cmake -S . -B build
cmake --build build
```

## Run

The argument is n, the number of tokens (a multiple of 64). The default is 16384.

```sh
./build/attention
./build/attention 32768
./build/flash
./build/flash 32768
./build/flash_tiled 32768
```
