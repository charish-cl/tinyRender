cmake_minimum_required(VERSION 3.29)
project(tinyRender)

set(CMAKE_CXX_STANDARD 20)

add_executable(tinyRender main.cpp tgaimage.cpp tgaimage.h)
