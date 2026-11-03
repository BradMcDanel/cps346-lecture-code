cmake_minimum_required(VERSION 3.10)

if(POLICY CMP0104)
  cmake_policy(SET CMP0104 NEW)
endif()

project(lecture-code LANGUAGES CUDA CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_STANDARD 11)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)
# Hide warnings from stb_image.h
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -O2 -diag-suppress 550")

include_directories(include)

add_executable(invert src/invert.cu)
add_executable(swirl src/swirl.cu)
add_executable(blur src/blur.cu)
add_executable(pixelate src/pixelate.cu)
