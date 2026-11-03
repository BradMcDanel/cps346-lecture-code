# Image filters

## Build

```sh
cd 17-image-filters
cmake -S . -B build
cmake --build build
```

## Run

Run from the `17-image-filters` folder. Each program writes a png into this folder.

```sh
./build/invert
./build/swirl 4
./build/blur 4
./build/pixelate 16
./build/pixelate 16 my-photo.jpg
```
