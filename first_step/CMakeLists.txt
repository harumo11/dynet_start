cmake_minimum_required(VERSION 3.12)

add_executable(first_step first_step)
add_executable(second_step second_step)

target_link_libraries(
	first_step 
	dynet 
	pthread
	)


target_link_libraries(
	second_step
	dynet 
	pthread
	)


