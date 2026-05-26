#!/bin/sh

# Sudo is only needed if there arn't any udev rules to give non-root access to the debug hardware
use_sudo=1
debug=0

openocd=$(which openocd)
sudo=$([ $use_sudo -ne 0 ] && echo "sudo")

if [ -z "$openocd" ]; then
	echo Could not find openocd in path
	exit 1
fi

openocd_prefix=`dirname $openocd`/..
openocd_script="$openocd_prefix/share/openocd/scripts"

if [ $debug -ne 0 ]; then
echo $sudo
echo $openocd
echo $openocd_prefix
echo $openocd_script
fi

$sudo $openocd "-f"{"$openocd_script/interface/stlink.cfg","$openocd_script/board/st_nucleo_f4.cfg"}
