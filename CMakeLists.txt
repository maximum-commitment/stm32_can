cmake_minimum_required(VERSION 4.2.3)
set(CMAKE_MAIN_PROJECT "stm32")


project(${CMAKE_MAIN_PROJECT} C ASM)
set(CMAKE_C_COMPILER /Applications/STM32CubeIDE.app/Contents/Eclipse/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.13.3.rel1.macos64_1.0.0.202411102158/tools/bin/arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER /Applications/STM32CubeIDE.app/Contents/Eclipse/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.13.3.rel1.macos64_1.0.0.202411102158/tools/bin/arm-none-eabi-gcc)

# Remove default linker flags that caused error, perhaps a better way in the future
unset(CMAKE_C_LINK_FLAGS)

# All files from source dir
file(GLOB MAIN_PROJECT_SOURCE_FILES Src/*.c)

# All files from drivers
set(DRIVER_SOURCE_FILES 
"./STM32CubeIDE/Application/User/syscalls.c"
"./STM32CubeIDE/Application/User/sysmem.c"
"./Drivers/BSP/Components/lis302dl/lis302dl.c"
"./Drivers/BSP/Components/lis3dsh/lis3dsh.c"
"./Drivers/BSP/STM32F4-Discovery/stm32f4_discovery.c"
"./Drivers/BSP/STM32F4-Discovery/stm32f4_discovery_accelerometer.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c"
"./Src/stm32f4xx_hal_msp.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_can.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_crc.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cryp.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dac.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dcmi.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_eth.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hash.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hcd.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2s.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_irda.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_iwdg.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_nand.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_nor.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pccard.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd_ex.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rng.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sd.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_smartcard.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sram.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_usart.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_wwdg.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_fsmc.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_sdmmc.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c")

set(ASM_FILES "STM32CubeIDE/Application/Startup/startup_stm32f407vgtx.s")

# Add executable target with source files created in above lists
set(STM32_ELF_FILE ${CMAKE_MAIN_PROJECT}.elf)
add_executable(${STM32_ELF_FILE})
target_sources(${STM32_ELF_FILE} PRIVATE ${ASM_FILES} ${MAIN_PROJECT_SOURCE_FILES} ${DRIVER_SOURCE_FILES})


set(CMAKE_EXE_LINKER_FLAGS "-mcpu=cortex-m4 -Wl,-T STM32CubeIDE/STM32F407VGTX_FLASH.ld --specs=nosys.specs -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group"
CACHE INTERNAL "linker options")

target_include_directories(${STM32_ELF_FILE} PRIVATE
"Inc"
"Drivers/CMSIS/Device/ST/STM32F4xx/Include"
"Drivers/STM32F4xx_HAL_Driver/Inc"
"Drivers/STM32F4xx_HAL_Driver/Inc/Legacy"
"Drivers/BSP/STM32F4-Discovery"
"Drivers/CMSIS/Include")

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
"-mthumb")

set(C_DEFS 
"-DDEBUG"
"-DSTM32F407xx"
"-DUSE_HAL_DRIVER"
"-DUSE_STM32F4_DISCO")

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
"-mthumb")

#arm-none-eabi-gcc -mcpu=cortex-m4 -g3 -DDEBUG -c -x assembler-with-cpp -MMD -MP -MF"Application/Startup/startup_stm32f407vgtx.d" 
#-MT"Application/Startup/startup_stm32f407vgtx.o" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "Application/Startup/startup_stm32f407vgtx.o" 
#"../Application/Startup/startup_stm32f407vgtx.s"


target_compile_options(${STM32_ELF_FILE} PRIVATE
$<$<COMPILE_LANGUAGE:C>:${C_FLAGS}>
$<$<COMPILE_LANGUAGE:ASM>:${ASM_FLAGS}>
)

target_compile_definitions(${STM32_ELF_FILE} PRIVATE
$<$<COMPILE_LANGUAGE:C>:${C_DEFS}>
$<$<COMPILE_LANGUAGE:ASM>:>
)

file(GENERATE
    OUTPUT "${CMAKE_BINARY_DIR}/objects.txt"
    CONTENT "$<TARGET_OBJECTS:${STM32_ELF_FILE}>\n"
)
