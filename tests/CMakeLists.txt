cmake_minimum_required(VERSION 3.31)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(char_test char_test.cpp)
target_link_libraries(char_test PRIVATE demo_deps GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(char_test)
