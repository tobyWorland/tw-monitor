set(CMAKE_SYSTEM_NAME "Generic")
set(CMAKE_C_COMPILER  "arm-none-eabi-gcc")
set(CMAKE_ASM_COMPILER  "arm-none-eabi-as")
set(CMAKE_C_FLAGS "-mcpu=cortex-m4 -mthumb -ffreestanding -g -Og -Wall -Wextra")
set(CMAKE_ASM_FLAGS "-g")
set(CMAKE_C_LINK_EXECUTABLE "arm-none-eabi-ld <LINK_FLAGS> <OBJECTS> -o <TARGET> <LINK_LIBRARIES>")

set(CMAKE_CXX_COMPILER "/bin/true")
set(CMAKE_CXX_LINK_EXECUTABLE "/bin/true")
