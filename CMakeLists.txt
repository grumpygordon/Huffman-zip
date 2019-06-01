cmake_minimum_required(VERSION 3.10)
project(huffman)

set(CMAKE_CXX_STANDARD 17)

add_executable(huflib main.cpp
        lib/huffman_lib.h
        lib/huffman_lib.cpp
        utility/utility.h
        utility/utility.cpp
        testing/testing.h
        testing/testing.cpp)
