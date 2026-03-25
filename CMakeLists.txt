cmake_minimum_required(VERSION 4.2.3)
project(stm32)


set(SOURCE_FILES main.c)

#add_compile_options(-g -Wall -Wextra -Wpedantic)

# Add executable target with source files listed in SOURCE_FILES variable
add_executable(stm32 ${SOURCE_FILES})


