cmake_minimum_required(VERSION 3.10)
project(lecture-code)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -O2 -Wextra")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(counter src/counter.cpp)
target_link_libraries(counter PRIVATE Threads::Threads)
target_compile_options(counter PRIVATE -O0)

add_executable(histogram src/histogram.cpp)
target_link_libraries(histogram PRIVATE Threads::Threads)
