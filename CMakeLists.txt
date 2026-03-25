cmake_minimum_required(VERSION 4.2.3)
set(CMAKE_MAIN_PROJECT "stm32")


project(${CMAKE_MAIN_PROJECT})
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
unset(CMAKE_C_LINK_FLAGS)

file(GLOB MAIN_PROJECT_SOURCE_FILES Src/*.c)
set(DRIVER_SOURCE_FILES "./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_cryp.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_irda.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_smartcard.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_cortex.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_pwr.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_dcmi.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_adc.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_hash.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_usart.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_dma.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_dac.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_can.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_tim_ex.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_pcd_ex.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_eth.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_hcd.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_rng.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_sram.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_gpio.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_crc.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_i2s.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_sd.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_rtc.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_spi.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_ll_sdmmc.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_tim.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_iwdg.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_pcd.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_uart.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_flash.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_ll_fsmc.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_nand.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_nor.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_i2c.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_wwdg.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_flash_ex.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_rcc.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_ll_usb.o"
"./STM32CubeIDE/Debug/Drivers/STM32F4xx_HAL_Drivers/stm32f4xx_hal_pccard.o"
"./STM32CubeIDE/Debug/Drivers/BSP/Components/lis3dsh.o"
"./STM32CubeIDE/Debug/Drivers/BSP/Components/lis302dl.o"
"./STM32CubeIDE/Debug/Drivers/BSP/STM32F4_DISCO/stm32f4_discovery.o"
"./STM32CubeIDE/Debug/Drivers/BSP/STM32F4_DISCO/stm32f4_discovery_accelerometer.o")


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
