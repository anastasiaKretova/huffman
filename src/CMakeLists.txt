cmake_minimum_required(VERSION 2.8)
project(huffman_src)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS: "${CMAKE_CXX_FLAGS} -Wall -std=c++14 -pedantic")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address -D_GLIBCXX_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -g")

add_library(huffman_src STATIC
        bit_seq.cpp
        bit_seq.h
        huffman.cpp
        huffman.h
        tree.cpp
        tree.h
        main.cpp)