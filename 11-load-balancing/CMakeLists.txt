cmake_minimum_required(VERSION 3.10)
project(lecture-code)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -O2 -Wextra")

find_package(OpenMP REQUIRED)
find_package(Threads REQUIRED)

add_executable(schedules src/schedules.cpp)
target_link_libraries(schedules PRIVATE OpenMP::OpenMP_CXX)

add_executable(thread_pool src/thread_pool.cpp)
target_link_libraries(thread_pool PRIVATE Threads::Threads)
target_compile_options(thread_pool PRIVATE -Wno-unused-parameter)
