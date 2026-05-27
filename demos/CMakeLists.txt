cmake_minimum_required(VERSION 3.31)

project(demos)
enable_language(C)

add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/libhost_monitor_redefined.a
	DEPENDS ../monitor_src/libhost_monitor.a
	COMMAND ${CMAKE_OBJCOPY} ARGS 
	--redefine-syms=${CMAKE_CURRENT_SOURCE_DIR}/symbol_rename_map
	../monitor_src/libhost_monitor.a libhost_monitor_redefined.a
)
add_library(host_monitor_redefined STATIC IMPORTED)
set_target_properties(host_monitor_redefined PROPERTIES IMPORTED_LOCATION
	 ${CMAKE_CURRENT_BINARY_DIR}/libhost_monitor_redefined.a)
target_sources(host_monitor_redefined INTERFACE
	 ${CMAKE_CURRENT_BINARY_DIR}/libhost_monitor_redefined.a)

add_library(demo_fakes STATIC
	fakes.c
	demo_support.c
)
target_compile_options(demo_fakes PUBLIC "-DHOST")

add_library(demo_deps INTERFACE)
target_link_libraries(demo_deps INTERFACE host_monitor_redefined demo_fakes)

function(add_demo name)
add_executable(demo_${name} ${name}.c)
target_link_libraries(demo_${name} PRIVATE demo_deps)
endfunction()

add_demo(test)
add_demo(gethexword)
add_demo(menu)
