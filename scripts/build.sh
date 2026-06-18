#!/bin/sh

cd `dirname $0`/..

ARM_TOOLCHAIN="-DCMAKE_TOOLCHAIN_FILE=cmake_toolchains/arm_cortex_m4.cmake"

mkdir -p builds
cmake -DCMAKE_BUILD_TYPE=Debug -B builds/host && cmake --build builds/host
cmake $ARM_TOOLCHAIN -DSRAM_ONLY=OFF -DCMAKE_BUILD_TYPE=MinSizeRel -B builds/target_flash  && cmake --build builds/target_flash
cmake $ARM_TOOLCHAIN -DSRAM_ONLY=ON  -DCMAKE_BUILD_TYPE=MinSizeRel -B builds/target_sram   && cmake --build builds/target_sram
cmake $ARM_TOOLCHAIN -DSRAM_ONLY=OFF -DCMAKE_BUILD_TYPE=Debug -B builds/debug_target_flash && cmake --build builds/debug_target_flash
cmake $ARM_TOOLCHAIN -DSRAM_ONLY=ON  -DCMAKE_BUILD_TYPE=Debug -B builds/debug_target_sram  && cmake --build builds/debug_target_sram
