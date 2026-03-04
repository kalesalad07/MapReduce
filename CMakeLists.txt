cmake_minimum_required(VERSION 3.10)
project(MapReduceFramework)

set(CMAKE_CXX_STANDARD 17)

# Tell CMake where the headers are
include_directories(include)

# Just build the example (it will pull in the header logic)
add_executable(wordcounter src/WordCount.cpp)
add_executable(invertedindex src/InvertedIndex.cpp)