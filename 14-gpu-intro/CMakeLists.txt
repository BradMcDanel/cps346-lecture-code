cmake_minimum_required(VERSION 3.10)

if(POLICY CMP0104)
  cmake_policy(SET CMP0104 NEW)
endif()

project(lecture-code LANGUAGES CUDA CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_STANDARD 11)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

add_executable(allocation src/allocation.cu)
add_executable(elementwise_add src/elementwise_add.cu)
