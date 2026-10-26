cmake_minimum_required(VERSION 3.10)

if(POLICY CMP0104)
  cmake_policy(SET CMP0104 NEW)
endif()

project(lecture-code LANGUAGES CUDA CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CUDA_STANDARD 11)
set(CMAKE_CUDA_STANDARD_REQUIRED ON)

add_executable(whoami src/whoami.cu)
add_executable(add_one src/add_one.cu)
add_executable(elementwise_add src/elementwise_add.cu)
add_executable(vector_add src/vector_add.cu)

# Optimize the host code too, so the CPU loop is a fair comparison.
target_compile_options(vector_add PRIVATE -O2)
