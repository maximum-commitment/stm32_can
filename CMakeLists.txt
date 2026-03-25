cmake_minimum_required(VERSION 4.2.3)
set(CMAKE_MAIN_PROJECT "stm32")


project(${CMAKE_MAIN_PROJECT})
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
unset(CMAKE_C_LINK_FLAGS)

file(GLOB MAIN_PROJECT_SOURCE_FILES Src/*.c)
set(DRIVER_SOURCE_FILES "./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cryp.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_irda.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_smartcard.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pwr.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dcmi.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_adc.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hash.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_usart.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dac.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_can.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd_ex.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_eth.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hcd.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rng.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sram.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_crc.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2s.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sd.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rtc.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_sdmmc.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_iwdg.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pcd.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_uart.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_fsmc.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_nand.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_nor.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_i2c.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_wwdg.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c"
"./Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_pccard.c"
"./Drivers/BSP/Components/lis3dsh/lis3dsh.c"
"./Drivers/BSP/Components/lis302dl/lis302dl.c"
"./Drivers/BSP/STM32F4-Discovery/stm32f4_discovery.c"
"./Drivers/BSP/STM32F4-Discovery/stm32f4_discovery_accelerometer.c"
"./STM32CubeIDE/Application/User/syscalls.c"
"./STM32CubeIDE/Application/User/sysmem.c"
)

#files missing from build
#STM32CubeIDE/Debug/Application/Startup/startup_stm32f407vgtx.o STM32CubeIDE/Debug/Application/User/stm32f4xx_hal_msp.o STM32CubeIDE/Debug/Drivers/CMSIS/system_stm32f4xx.o


# Add executable target with source files listed in MAIN_PROJECT_SOURCE_FILES variable
add_executable(${CMAKE_MAIN_PROJECT} ${MAIN_PROJECT_SOURCE_FILES} ${DRIVER_SOURCE_FILES})

target_sources(${CMAKE_MAIN_PROJECT} PRIVATE ${MAIN_PROJECT_SOURCE_FILES} ${DRIVER_SOURCE_FILES})


set(CMAKE_EXE_LINKER_FLAGS "-mcpu=cortex-m4 -Wl,-T STM32CubeIDE/STM32F407VGTX_FLASH.ld --specs=nosys.specs -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,--end-group"
CACHE INTERNAL "linker options")

target_include_directories(${CMAKE_MAIN_PROJECT} PRIVATE
"Inc" 
"Drivers/CMSIS/Device/ST/STM32F4xx/Include"
"Drivers/STM32F4xx_HAL_Driver/Inc"
"Drivers/BSP/STM32F4-Discovery"
"Drivers/CMSIS/Include")

target_compile_definitions(${CMAKE_MAIN_PROJECT} PRIVATE "DEBUG" "STM32F407xx" "USE_HAL_DRIVER" "USE_STM32F4_DISCO")
target_compile_options(${CMAKE_MAIN_PROJECT} PRIVATE 
"-mcpu=cortex-m4" 
"-std=gnu11"
"-g3"
"-O0"
"-ffunction-sections"
"-fdata-sections"
"-Wall"
"-fstack-usage"
"--specs=nano.specs"
"-mfpu=fpv4-sp-d16"
"-mfloat-abi=hard"
"-mthumb")

message(STATUS "${CMAKE_C_LINK_FLAGS}")
message(STATUS "Link options for stm32: ${CMAKE_EXE_LINKER_FLAGS}")
