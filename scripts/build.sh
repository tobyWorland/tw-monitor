#!/bin/sh

cd `dirname $0`/..

mkdir -p builds
cmake -DCMAKE_BUILD_TYPE=Debug -B builds/host && cmake --build builds/host
cmake -DSRAM_ONLY=OFF -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=cmake_toolchains/arm_cortex_m4.cmake -B builds/target_flash && cmake --build builds/target_flash
cmake -DSRAM_ONLY=ON -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=cmake_toolchains/arm_cortex_m4.cmake -B builds/target_sram && cmake --build builds/target_sram
