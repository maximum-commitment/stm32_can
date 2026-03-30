cmake_minimum_required(VERSION 4.2.3)
set(CMAKE_MAIN_PROJECT "stm32")


project(${CMAKE_MAIN_PROJECT} C ASM)
link_directories(.)

set(ARM_GCC_PATH "/Applications/STM32CubeIDE.app/Contents/Eclipse/plugins/com.st.stm32cube.ide.mcu.externaltools.gnu-tools-for-stm32.13.3.rel1.macos64_1.0.0.202411102158")

set(CMAKE_C_COMPILER ${ARM_GCC_PATH}/tools/bin/arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER ${ARM_GCC_PATH}/tools/bin/arm-none-eabi-gcc)
set(CMAKE_AR ${ARM_GCC_PATH}/tools/bin/arm-none-eabi-ar)

# Remove default linker flags that caused error, perhaps a better way in the future
unset(CMAKE_C_LINK_FLAGS)

set(CMAKE_C_CREATE_STATIC_LIBRARY "<CMAKE_AR> rc <TARGET> <OBJECTS> ")

set(HAL_DRIVER_PATH "Drivers/STM32F4xx_HAL_Driver")
set(BSP_DRIVER_PATH "Drivers/BSP")
set(MAIN_PROJECT_SOURCE_PATH "Src")

# All files from source dir
file(GLOB MAIN_PROJECT_SOURCE ${MAIN_PROJECT_SOURCE_PATH}/*.c)

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
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_gpio.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_pwr.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_rcc.c"
"${HAL_DRIVER_PATH}/Src/stm32f4xx_hal_exti.c"
)

set(ASM_SOURCE "Startup/startup_stm32f407vgtx.s")

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

set(FREERTOS_KERNEL_SOURCE_PATH FreeRTOS)

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
${FREERTOS_KERNEL_SOURCE_PATH}/include
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

target_include_directories(${STM32_ELF_FILE} PRIVATE
"Inc"
"Drivers/CMSIS/Device/ST/STM32F4xx/Include"
"${HAL_DRIVER_PATH}/Inc"
"Drivers/CMSIS/Include"
${FREERTOS_KERNEL_SOURCE_PATH}/include
)

set(CMAKE_EXE_LINKER_FLAGS "-mcpu=cortex-m4 -Wl,-T STM32F407VGTX_FLASH.ld --specs=nosys.specs -Wl,--gc-sections -static --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -Wl,--start-group -lc -lm -Wl,-L. -Wl,--end-group"
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

set(CPPCHECK_FILES "")
foreach(cfile IN ITEMS ${MAIN_PROJECT_SOURCE})
get_target_property(CPPCHECK_INCLUDES ${STM32_ELF_FILE} INCLUDE_DIRECTORIES)
get_filename_component(cfile_no_ext ${cfile} NAME_WLE)
list(TRANSFORM CPPCHECK_INCLUDES PREPEND -I)
set(cppcheckfile "${MAIN_PROJECT_SOURCE_PATH}/${cfile_no_ext}.cppcheck.txt")
message(${cppcheckfile})
list(APPEND CPPCHECK_FILES ${cppcheckfile})

add_custom_command(
OUTPUT ${cppcheckfile}
COMMAND cppcheck --report-type=misra-c-2012 --output-file=${cppcheckfile} --template="{file}:{line}: {severity}: {message} [{id}]" --enable=all 
    ${CPPCHECK_INCLUDES}
    -D__GNUC__ ${C_DEFS}
    ${cfile}
DEPENDS ${STM32_ELF_FILE}
COMMENT "Create cppcheck text file"
VERBATIM
)
endforeach()

# Custom target to format code
add_custom_target(analysis DEPENDS ${CPPCHECK_FILES}
COMMENT "Running cppcheck for static analysis..."
)

target_link_libraries(stm32.elf freertos_kernel)

file(GENERATE
    OUTPUT "${CMAKE_BINARY_DIR}/objects.txt"
    CONTENT "$<TARGET_OBJECTS:${STM32_ELF_FILE}>\n"
)