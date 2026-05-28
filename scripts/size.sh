#!/bin/sh

cd `dirname $0`/..

build="$1"

case "$build" in
	sram|flash) ;;
	*)
	echo "Usage: $0 <sram|flash>"
	exit 1
esac

arm-none-eabi-size `find builds/target_$build/monitor_src/ -name "*.obj"`
arm-none-eabi-size builds/target_$build/monitor_src/monitor_$build
