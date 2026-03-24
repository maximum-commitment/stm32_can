################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/Users/charlesmurphy/STM32CubeIDE/workspace_1.18.0/Demonstrations/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c \
/Users/charlesmurphy/STM32CubeIDE/workspace_1.18.0/Demonstrations/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c \
/Users/charlesmurphy/STM32CubeIDE/workspace_1.18.0/Demonstrations/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c 

OBJS += \
./Middlewares/STM32_USBD_Library/Core/usbd_core.o \
./Middlewares/STM32_USBD_Library/Core/usbd_ctlreq.o \
./Middlewares/STM32_USBD_Library/Core/usbd_ioreq.o 

C_DEPS += \
./Middlewares/STM32_USBD_Library/Core/usbd_core.d \
./Middlewares/STM32_USBD_Library/Core/usbd_ctlreq.d \
./Middlewares/STM32_USBD_Library/Core/usbd_ioreq.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/STM32_USBD_Library/Core/usbd_core.o: /Users/charlesmurphy/STM32CubeIDE/workspace_1.18.0/Demonstrations/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_core.c Middlewares/STM32_USBD_Library/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_STM32F4_DISCO -DSTM32F407xx -DUSE_HAL_DRIVER -c -I../../Inc -I../../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../../Drivers/STM32F4xx_HAL_Driver/Inc -I../../Drivers/BSP/STM32F4-Discovery -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/HID/Inc -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/STM32_USBD_Library/Core/usbd_ctlreq.o: /Users/charlesmurphy/STM32CubeIDE/workspace_1.18.0/Demonstrations/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ctlreq.c Middlewares/STM32_USBD_Library/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_STM32F4_DISCO -DSTM32F407xx -DUSE_HAL_DRIVER -c -I../../Inc -I../../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../../Drivers/STM32F4xx_HAL_Driver/Inc -I../../Drivers/BSP/STM32F4-Discovery -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/HID/Inc -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
Middlewares/STM32_USBD_Library/Core/usbd_ioreq.o: /Users/charlesmurphy/STM32CubeIDE/workspace_1.18.0/Demonstrations/Middlewares/ST/STM32_USB_Device_Library/Core/Src/usbd_ioreq.c Middlewares/STM32_USBD_Library/Core/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_STM32F4_DISCO -DSTM32F407xx -DUSE_HAL_DRIVER -c -I../../Inc -I../../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../../Drivers/STM32F4xx_HAL_Driver/Inc -I../../Drivers/BSP/STM32F4-Discovery -I../../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../../Middlewares/ST/STM32_USB_Device_Library/Class/HID/Inc -I../../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-STM32_USBD_Library-2f-Core

clean-Middlewares-2f-STM32_USBD_Library-2f-Core:
	-$(RM) ./Middlewares/STM32_USBD_Library/Core/usbd_core.cyclo ./Middlewares/STM32_USBD_Library/Core/usbd_core.d ./Middlewares/STM32_USBD_Library/Core/usbd_core.o ./Middlewares/STM32_USBD_Library/Core/usbd_core.su ./Middlewares/STM32_USBD_Library/Core/usbd_ctlreq.cyclo ./Middlewares/STM32_USBD_Library/Core/usbd_ctlreq.d ./Middlewares/STM32_USBD_Library/Core/usbd_ctlreq.o ./Middlewares/STM32_USBD_Library/Core/usbd_ctlreq.su ./Middlewares/STM32_USBD_Library/Core/usbd_ioreq.cyclo ./Middlewares/STM32_USBD_Library/Core/usbd_ioreq.d ./Middlewares/STM32_USBD_Library/Core/usbd_ioreq.o ./Middlewares/STM32_USBD_Library/Core/usbd_ioreq.su

.PHONY: clean-Middlewares-2f-STM32_USBD_Library-2f-Core

