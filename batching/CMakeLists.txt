cmake_minimum_required(VERSION 3.10)

add_executable(train_xor_batch train_xor_batch.cpp)
add_executable(test_xor_batch test_xor_batch.cpp)

target_link_libraries(train_xor_batch dynet pthread Gpop)
target_link_libraries(test_xor_batch dynet pthread Gpop)
