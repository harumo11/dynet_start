cmake_minimum_required(VERSION 3.10)

add_definitions("-std=c++17")

add_executable(iris iris.cpp)
#add_executable(iris_data_test iris_data_test.cpp)

target_link_libraries(iris dynet pthread gpop)
