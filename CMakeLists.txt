cmake_minimum_required(VERSION 3.10)
project(Huffman)

set(CMAKE_CXX_STANDARD 11)

if (CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pedantic")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -O3")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
endif ()

include_directories(lib util)

add_library(huflib STATIC
        lib/decode.cpp lib/tree.cpp lib/encode.cpp)

add_executable(HUFFMAN util/console_utility.cpp)

target_link_libraries(HUFFMAN huflib)