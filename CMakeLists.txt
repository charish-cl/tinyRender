cmake_minimum_required(VERSION 3.29)
project(tinyRender)

set(CMAKE_CXX_STANDARD 20)
# ???????
set(SOURCE_DIR "C:/Users/Administrator/Desktop/Feng/tinyRender")
add_executable(tinyRender main.cpp model.cpp model.h geometry.h tgaimage.cpp tgaimage.h)
