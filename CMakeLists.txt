cmake_minimum_required(VERSION 3.27.5)

project(byte-buffer-lib VERSION 1.0.0)
set(CMAKE_CXX_STANDARD 20)

add_library(byte_buffer SHARED byte_buffer/byte_buffer.cpp)
target_include_directories(byte_buffer INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
