cmake_minimum_required(VERSION 3.31)

project(monitor)
enable_language(C ASM)

set(C_SOURCE
	assert.c
	char.c
	io.c
	menu.c
	monitor.c
	scb.c
	vt.c
)

set(ASM_SOURCE
	hardware.s
	startup.s
	vector.s
)

if(CROSS_COMPILING)
set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/link_sram.ld")
set(CMAKE_EXE_LINKER_FLAGS "-T ${LINKER_SCRIPT}")

add_executable(monitor
	${C_SOURCE}
	${ASM_SOURCE}
)

set_target_properties(monitor PROPERTIES LINK_DEPENDS ${LINKER_SCRIPT})
else()
add_library(host_monitor STATIC
	${C_SOURCE}
)
# Not much point in not having this be PRIVATE as the options won't survive the redefining in demos
target_compile_options(host_monitor PRIVATE "-DHOST -ffreestanding")
endif()
