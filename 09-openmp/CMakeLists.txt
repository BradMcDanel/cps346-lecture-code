cmake_minimum_required(VERSION 3.10)
project(lecture-code)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -O2 -Wextra")

find_package(OpenMP REQUIRED)

add_executable(hello src/hello.cpp)
target_link_libraries(hello PRIVATE OpenMP::OpenMP_CXX)

add_executable(sum src/sum.cpp)
target_link_libraries(sum PRIVATE OpenMP::OpenMP_CXX)

add_executable(monte_carlo src/monte_carlo.cpp)
target_link_libraries(monte_carlo PRIVATE OpenMP::OpenMP_CXX)

add_executable(downsample src/downsample.cpp)
target_link_libraries(downsample PRIVATE OpenMP::OpenMP_CXX)
