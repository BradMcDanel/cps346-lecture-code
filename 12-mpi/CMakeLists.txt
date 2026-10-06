cmake_minimum_required(VERSION 3.10)
project(lecture-code)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -O2 -Wextra")

find_package(MPI REQUIRED)

add_executable(hello src/hello.cpp)
target_link_libraries(hello PRIVATE MPI::MPI_CXX)

add_executable(ping_pong src/ping_pong.cpp)
target_link_libraries(ping_pong PRIVATE MPI::MPI_CXX)
target_compile_options(ping_pong PRIVATE -Wno-unused-parameter)
