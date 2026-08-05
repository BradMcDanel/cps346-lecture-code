cmake_minimum_required(VERSION 3.10)
project(lecture-code)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -O2 -Wextra")

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

add_executable(estimate_pi src/estimate_pi.cpp)
target_link_libraries(estimate_pi PRIVATE Threads::Threads)

add_executable(fetch_pages src/fetch_pages.cpp)
target_link_libraries(fetch_pages PRIVATE Threads::Threads)
