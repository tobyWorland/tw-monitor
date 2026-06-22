
#!/bin/sh

`dirname $0`/quick_openocd.sh -c "init" -c "reset halt" -c "program `dirname $0`/../builds/target_flash/monitor/monitor_flash preverify verify exit" -c "shutdown"
