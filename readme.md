# Monitor

On chip stepping debugger, inspecting hardware and modifying memory
interactivity over the UART. Similar to those found on vintage home
computers.

This was mostly written to explore using the debug monitor exception
available on some ARM cores, and to experiment with interactivity
poking the hardware and assembling some instructions on the MCU
itself.

* Target code is written in C and Thumb assembly
* Unit test code is written in C and C++ with googletest

NOTE: Currently only a subset of the instruction set is supported for
assmebling and disassembling.

## Supported targets (so far)
* NUCLEO-F411RE

## Build Dependencies
* Binutils (--target arm-none-eabi)
* GCC (--target arm-none-eabi)
* CMake
* GoogleTest (optional - testing only)

## Misc Dependencies
* [meta-bfield](https://github.com/tobyWorland/meta-bfield) (1)
* Python3 & matplotlib (optional - generating piechart breakdown of large object files)

(1) Pragmatically generates the C source for encoding and decoding thumb instructions. Written for this project. Output is stored in `monitor/autogen_encodings`

## Quick Build

```
git submodule update --init
./scripts/build.sh
```

1. You will want to flash `builds/target_flash/monitor/monitor_flash`
to your board.
2. Check the serial console (115200bps, no parity, one stop bit, no flow control) for the menu.
3. Disconnect your debugger and reset, you may need to cut power.
4. If the serial terminal reports "Halting debug? YES" try step 3 again. This means the hardware debugger will take priority and the debug monitor exception will NOT get triggered on breakpoints.

Hit '?' to get a list of actions when given a prompt.
