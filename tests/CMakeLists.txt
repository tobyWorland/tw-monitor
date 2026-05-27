cmake_minimum_required(VERSION 3.31)

add_executable(char_test test.c)
target_link_libraries(char_test PRIVATE demo_deps)
