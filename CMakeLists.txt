cmake_minimum_required(VERSION 3.13)
project(huffamn)

set(CMAKE_CXX_STANDARD 17)

add_executable(huflib main.cpp
        compress_testing.cpp
        compress_testing.h
        lib/compress_lib.h
        lib/codes.cpp
        utility/file_decompressor.cpp
        utility/file_compressor.cpp utility/utility.h)