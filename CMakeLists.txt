cmake_minimum_required(VERSION 3.31)

project(toplevel)
enable_language(C ASM)

add_subdirectory(monitor_src)
if(NOT CMAKE_CROSSCOMPILING)
add_subdirectory(demos)
add_subdirectory(tests)
endif()
