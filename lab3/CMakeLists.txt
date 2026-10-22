cmake_minimum_required(VERSION 3.10)
project(lab2)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-Wall -Wextra)

add_executable(
    program 
    src/det_single_threaded.c 
    src/free_matrix.c 
    src/main.c 
    src/read_file.c 
    src/bench_single_threaded.c
    )