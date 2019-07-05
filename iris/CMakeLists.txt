cmake_minimum_required(VERSION 3.10)

add_definitions("-std=c++17")
set(CMAKE_CXX_COMPILER clang++-6.0)
set(CMAKE_CXX_GLAGS "-g")

add_executable(iris iris.cpp)

target_link_libraries(iris dynet pthread gpop)
