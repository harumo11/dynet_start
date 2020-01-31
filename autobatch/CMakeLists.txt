cmake_minimum_required(VERSION 3.10)

add_executable(train_xor_autobatch train_xor_autobatch.cpp)
target_link_libraries(train_xor_autobatch dynet pthread)
