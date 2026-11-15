cmake_minimum_required(VERSION 3.10)

if(POLICY CMP0104)
  cmake_policy(SET CMP0104 NEW)
endif()

project(lecture-code LANGUAGES CUDA CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_STANDARD 11)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -O2")

include_directories(include)

add_executable(attention src/attention.cu)
add_executable(flash src/flash.cu)
add_executable(flash_tiled src/flash_tiled.cu)
