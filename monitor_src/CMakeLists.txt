cmake_minimum_required(VERSION 3.31)

project(monitor)
enable_language(C ASM)

set(C_SOURCE
    arm.c
    assert.c
    char.c
    io.c
    menu.c
    monitor.c
    monitor_cmds/monitor_call.c
    monitor_cmds/monitor_disassemble.c
    monitor_cmds/monitor_enter.c
    monitor_cmds/monitor_memdump.c
    scb.c
    string.c
    thumb_asm.c
    vt.c
)

set(ASM_SOURCE
    hardware.s
    startup.s
    vector.s
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
    ${ASM_SOURCE}
)

set_target_properties(${MONITOR_NAME} PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})
else()
add_library(host_monitor STATIC
    ${C_SOURCE}
)
# Not much point in not having this be PRIVATE as the options won't survive the redefining in demos
target_compile_options(host_monitor PRIVATE "-DHOST -ffreestanding")
endif()
