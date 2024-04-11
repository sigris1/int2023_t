cmake_minimum_required(VERSION 3.12)

project(int2023_t)


set(CMAKE_CXX_STANDARD 20)

add_subdirectory(lib)

enable_testing()
add_subdirectory(tests)