cmake_minimum_required(VERSION 3.10)
project(huffman)

set(CMAKE_CXX_STANDARD 17)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fsanitize=address,undefined -g -std=c++11 -pedantic")
endif()


add_executable(huflib main.cpp
        lib/huffman_lib.h
        lib/huffman_lib.cpp
        lib/huffman_encoder.h
        lib/huffman_encoder.cpp
        lib/huffman_decoder.h
        lib/huffman_decoder.cpp
        utility/utility.h
        utility/utility.cpp
        testing/testing.h
        testing/testing.cpp)
