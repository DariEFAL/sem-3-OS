cmake_minimum_required(VERSION 3.10)
project(lab2)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra)

add_executable(parent src/parent.cpp)
add_executable(child  src/child.cpp)