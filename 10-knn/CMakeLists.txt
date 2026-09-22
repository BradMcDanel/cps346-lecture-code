cmake_minimum_required(VERSION 3.10)
project(lecture-code)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -O2 -Wextra")

find_package(OpenMP REQUIRED)

add_executable(mnist_info src/mnist_info.cpp src/mnist.cpp)

add_executable(knn src/knn.cpp src/mnist.cpp)
target_link_libraries(knn PRIVATE OpenMP::OpenMP_CXX)
target_compile_options(knn PRIVATE -Wno-unused-parameter)
