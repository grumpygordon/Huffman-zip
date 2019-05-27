cmake_minimum_required(VERSION 3.10)
project(huffman)

set(CMAKE_CXX_STANDARD 17)

add_executable(huflib main.cpp
        compress_testing.cpp
        compress_testing.h
        lib/huffman_lib.h
        lib/huffman_lib.cpp
        utility/utility.h
        utility/utility.cpp)
