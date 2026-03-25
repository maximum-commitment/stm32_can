cmake_minimum_required(VERSION 4.2.3)
project(stm32)


set(SOURCE_FILES Src/main.c)

#add_compile_options(-g -Wall -Wextra -Wpedantic)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(stm32 ${SOURCE_FILES})

set(CMAKE_C_COMPILER arm-none-eabi-gcc)

#include(toolchain_arm-none-eabi.cmake)




include_directories(
"Inc" 
"Drivers/CMSIS/Device/ST/STM32F4xx/Include"
"Drivers/STM32F4xx_HAL_Driver/Inc"
"Drivers/BSP/STM32F4-Discovery"
"Drivers/CMSIS/Include")

add_compile_definitions("DEBUG" "STM32F407xx" "USE_HAL_DRIVER" "USE_STM32F4_DISCO")
#    set(CMAKE_EXE_LINKER_FLAGS "-shared -Bsymbolic -L/usr/lib -T${GNU_EFI_LSCRIPT}")
#    set(LD_START_GROUP ${GNU_EFI_CRT0})
#    set(LD_END_GROUP "-lgnuefi -lefi")
#    set(UPDATE_SOURCES src/update_ram.c)

