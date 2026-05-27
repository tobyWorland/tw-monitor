cmake_minimum_required(VERSION 3.31)

project(toplevel)
enable_language(C ASM)

add_subdirectory(monitor_src)
if(NOT CROSS_COMPILING)
add_subdirectory(demos)
endif()
