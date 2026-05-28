cmake_minimum_required(VERSION 3.31)

project(toplevel)
enable_language(C ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(monitor_src)
if(NOT CMAKE_CROSSCOMPILING)
add_subdirectory(demos)

add_subdirectory(third_party/googletest EXCLUDE_FROM_ALL)
enable_testing()
add_subdirectory(tests)
endif()
