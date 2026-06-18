cmake_minimum_required(VERSION 3.31)

project(toplevel)
enable_language(C ASM)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(NOT CMAKE_BUILD_TYPE)
message(WARNING "Build type not given, defaulting to DEBUG")
set(CMAKE_BUILD_TYPE "Debug")
endif()

add_subdirectory(monitor)
if(NOT CMAKE_CROSSCOMPILING)
add_subdirectory(demos)
add_subdirectory(tools)

add_subdirectory(third_party/googletest EXCLUDE_FROM_ALL)
enable_testing()
add_subdirectory(tests)
endif()
