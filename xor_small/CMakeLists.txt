cmake_minimum_required(VERSION 3.12)

add_executable(xor_small xor_small.cpp)

target_link_libraries(xor_small dynet pthread gpop)
