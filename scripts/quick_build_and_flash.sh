#!/bin/sh

cd `dirname $0`/..

ARM_TOOLCHAIN="-DCMAKE_TOOLCHAIN_FILE=cmake_toolchains/arm_cortex_m4.cmake"

mkdir -p builds

# Build
cmake $ARM_TOOLCHAIN -DSRAM_ONLY=OFF -DCMAKE_BUILD_TYPE=MinSizeRel -B builds/target_flash && \
    cmake --build builds/target_flash

# Flash board
echo Flashing with openocd...
./scripts/flash.sh
