#!/bin/sh

cd `dirname $0`/..

mkdir -p builds
cmake -DCMAKE_BUILD_TYPE=Debug -B builds/host && cmake --build builds/host
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=cmake_toolchains/arm_cortex_m4.cmake -B builds/target && cmake --build builds/target
