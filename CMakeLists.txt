cmake_minimum_required(VERSION 4.2.3)
set(CMAKE_MAIN_PROJECT "stm32")


project(${CMAKE_MAIN_PROJECT} C ASM)
link_directories(.)

set(CMAKE_C_COMPILER /Applications/STM32CubeIDE.app/Contents/Eclipse/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.13.3.rel1.macos64_1.0.0.202411102158/tools/bin/arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER /Applications/STM32CubeIDE.app/Contents/Eclipse/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.13.3.rel1.macos64_1.0.0.202411102158/tools/bin/arm-none-eabi-gcc)
set(CMAKE_AR /Applications/STM32CubeIDE.app/Contents/Eclipse/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.13.3.rel1.macos64_1.0.0.202411102158/tools/bin/arm-none-eabi-ar)

# Remove default linker flags that caused error, perhaps a better way in the future
unset(CMAKE_C_LINK_FLAGS)

set(CMAKE_C_CREATE_STATIC_LIBRARY "<CMAKE_AR> rc <TARGET> <OBJECTS> ")

set(HAL_DRIVER_PATH "Drivers/STM32F4xx_HAL_Driver")
set(BSP_DRIVER_PATH "Drivers/BSP")

# All files from source dir
file(GLOB MAIN_PROJECT_SOURCE Src/*.c)

set(BSP_DRIVER_SOURCE
"${BSP_DRIVER_PATH}/Components/lis302dl/lis302dl.c"
"${BSP_DRIVER_PATH}/Components/lis3dsh/lis3dsh.c"
)

# All files from drivers
set(HAL_DRIVER_SOURCE
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_adc.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_can.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_cortex.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_crc.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_cryp.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_dac.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_dcmi.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_dma.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_eth.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_flash.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_flash_ex.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_gpio.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_hash.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_hcd.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_i2c.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_i2s.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_irda.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_iwdg.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_nand.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_nor.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_pccard.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_pcd.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_pcd_ex.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_pwr.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_rcc.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_rng.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_rtc.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_sd.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_smartcard.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_spi.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_sram.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_tim.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_tim_ex.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_uart.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_usart.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_wwdg.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_ll_fsmc.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_ll_sdmmc.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_ll_usb.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_exti.c"
)

set(C_FLAGS
"-mcpu=cortex-m4"
"-std=gnu11"
"-g3"
"-O0"
"-ffunction-sections"
"-fdata-sections"
"-Wall"
"-fstack-usage"
"-fcyclomatic-complexity"
"--specs=nano.specs"
"-mfpu=fpv4-sp-d16"
"-mfloat-abi=hard"
"-mthumb"
)

set(C_DEFS 
"-DDEBUG"
"-DSTM32F407xx"
"-DUSE_HAL_DRIVER"
"-DUSE_STM32F4_DISCO"
)

set(ASM_FLAGS 
"-mcpu=cortex-m4"
"-g3"
"-DDEBUG"
"-c"
"-x"
"assembler-with-cpp"
"--specs=nano.specs"
"-mfpu=fpv4-sp-d16"
"-mfloat-abi=hard"
"-mthumb"
)

set(FREERTOS_KERNEL_PATH "../FreeRTOS/FreeRTOS")
set(FREERTOS_KERNEL_SOURCE_PATH "${FREERTOS_KERNEL_PATH}/Source")

#set(stm32_elf_EXTERNAL_OBJECTS libfreertos_kernel.a)
#set_target_properties(stm32.elf PROPERTIES EXTERNAL_OBJECTS "libfreertos_kernel.a")

set(ASM_SOURCE "STM32CubeIDE/Application/Startup/startup_stm32f407vgtx.s")

add_library(freertos_kernel STATIC)

target_sources(freertos_kernel PRIVATE
    ${FREERTOS_KERNEL_SOURCE_PATH}/croutine.c
    ${FREERTOS_KERNEL_SOURCE_PATH}/event_groups.c
    ${FREERTOS_KERNEL_SOURCE_PATH}/list.c
    ${FREERTOS_KERNEL_SOURCE_PATH}/queue.c
    ${FREERTOS_KERNEL_SOURCE_PATH}/stream_buffer.c
    ${FREERTOS_KERNEL_SOURCE_PATH}/tasks.c
    ${FREERTOS_KERNEL_SOURCE_PATH}/timers.c
)

target_include_directories(freertos_kernel PRIVATE
"Inc"
"Drivers/CMSIS/Device/ST/STM32F4xx/Include"
"${HAL_DRIVER_PATH}/Inc"
"Drivers/CMSIS/Include"
${FREERTOS_KERNEL_SOURCE_PATH}/include
${FREERTOS_KERNEL_SOURCE_PATH}/portable/GCC/ARM_CM4F
)

target_compile_options(freertos_kernel PRIVATE
$<$<COMPILE_LANGUAGE:C>:${C_FLAGS}>
$<$<COMPILE_LANGUAGE:ASM>:${ASM_FLAGS}>
)

target_compile_definitions(freertos_kernel PRIVATE
$<$<COMPILE_LANGUAGE:C>:${C_DEFS}>
$<$<COMPILE_LANGUAGE:ASM>:>
)

# Add executable target with source files created in above lists
set(STM32_ELF_FILE ${CMAKE_MAIN_PROJECT}.elf)
add_executable(${STM32_ELF_FILE})

target_sources(${STM32_ELF_FILE} PRIVATE 
${ASM_SOURCE}
${MAIN_PROJECT_SOURCE}
${HAL_DRIVER_SOURCE}
${BSP_DRIVER_SOURCE}
)

set_target_properties(${STM32_ELF_FILE} PROPERTIES
    LINK_FLAGS "libfreertos_kernel.a"
)

target_include_directories(${STM32_ELF_FILE} PRIVATE
"Inc"
"Drivers/CMSIS/Device/ST/STM32F4xx/Include"
"${HAL_DRIVER_PATH}/Inc"
"${BSP_DRIVER_PATH}/STM32F4-Discovery"
"Drivers/CMSIS/Include"
${FREERTOS_KERNEL_SOURCE_PATH}/include
${FREERTOS_KERNEL_SOURCE_PATH}/portable/GCC/ARM_CM4F
Common/include
CORTEX_M4F_STM32F407ZG-SK/board
CORTEX_M4F_STM32F407ZG-SK
)

set(CMAKE_EXE_LINKER_FLAGS "-mcpu=cortex-m4 -Wl,-T STM32CubeIDE/STM32F407VGTX_FLASH.ld --specs=nosys.specs -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,-L. -Wl,--end-group"
CACHE INTERNAL "linker options"
)

target_compile_options(${STM32_ELF_FILE} PRIVATE
$<$<COMPILE_LANGUAGE:C>:${C_FLAGS}>
$<$<COMPILE_LANGUAGE:ASM>:${ASM_FLAGS}>
)

target_compile_definitions(${STM32_ELF_FILE} PRIVATE
$<$<COMPILE_LANGUAGE:C>:${C_DEFS}>
$<$<COMPILE_LANGUAGE:ASM>:>
)

target_link_libraries(stm32.elf freertos_kernel)

file(GENERATE
    OUTPUT "${CMAKE_BINARY_DIR}/objects.txt"
    CONTENT "$<TARGET_OBJECTS:${STM32_ELF_FILE}>\n"
)