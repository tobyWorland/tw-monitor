cmake_minimum_required(VERSION 3.31)

project(monitor)
enable_language(C ASM)

set(C_SOURCE
    arm/arm.c
    assert.c
    char.c
    io.c
    memory.c
    menu.c
    monitor.c
    monitor_cmds/monitor_assemble.c
    monitor_cmds/monitor_call.c
    monitor_cmds/monitor_dev.c
    monitor_cmds/monitor_disassemble.c
    monitor_cmds/monitor_enter.c
    monitor_cmds/monitor_memory.c
    monitor_cmds/monitor_memdump.c
    string.c
    syscall.c
    thumb_asm.c
    vt.c
    terminal.c
    transfer.c

    autogen_encodings/t32_encoding.c
)

# Always set -Os on autogen encoding files regardless of build type
set_source_files_properties(
    autogen_encodings/t32_encoding.c
    PROPERTIES COMPILE_OPTIONS "-Os"
)

set(TARGET_SOURCE
    startup.s
    vector.s
    vector.c
    clobber_sram.s
    arm/debug.c
    arm/scb.c
    hardware/stm32f411xce/stm32f411xce_board.c
    hardware/stm32f411xce/stm32f411xce_gpio.c
    hardware/stm32f411xce/stm32f411xce_peripherals.c
    hardware/stm32f411xce/stm32f411xce_rcc.c
    hardware/stm32f411xce/stm32f411xce_timer.c
    hardware/stm32f411xce/stm32f411xce_usart.c
)

if(CMAKE_CROSSCOMPILING)

option(SRAM_ONLY "Don't store anything in flash (development only)" OFF)
message("SRAM_ONLY=${SRAM_ONLY}")

if (SRAM_ONLY)
set(MONITOR_NAME "monitor_sram")
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/link_sram.ld")
set(CMAKE_ASM_FLAGS "${CMAKE_ASM_FLAGS} --defsym STARTUP_FROM_SRAM=1")
else()
set(MONITOR_NAME "monitor_flash")
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/link_flash.ld")
endif()

set(CMAKE_EXE_LINKER_FLAGS "-T ${LINKER_SCRIPT}")

add_executable(${MONITOR_NAME}
    ${C_SOURCE}
    ${TARGET_SOURCE}
)

set_target_properties(${MONITOR_NAME} PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})

add_custom_command(TARGET ${MONITOR_NAME}
    POST_BUILD
    COMMAND ${CMAKE_OBJCOPY}
    ARGS
    -O binary
    ${MONITOR_NAME} ${MONITOR_NAME}.bin
)


else()
add_library(host_monitor STATIC
    ${C_SOURCE}
)
# Not much point in not having this be PRIVATE as the options won't survive the redefining in demos
target_compile_options(host_monitor PRIVATE -ffreestanding)
target_compile_definitions(host_monitor PRIVATE HOST)
endif()
