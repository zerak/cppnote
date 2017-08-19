cmake_minimum_required(VERSION 3.8)
project(cppnote)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp array.cpp lambda.cpp)
add_executable(cppnote ${SOURCE_FILES})