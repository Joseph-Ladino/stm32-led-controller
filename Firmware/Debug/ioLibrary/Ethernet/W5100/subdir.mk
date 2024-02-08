################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ioLibrary/Ethernet/W5100/w5100.c 

OBJS += \
./ioLibrary/Ethernet/W5100/w5100.o 

C_DEPS += \
./ioLibrary/Ethernet/W5100/w5100.d 


# Each subdirectory must supply rules for building sources it contributes
ioLibrary/Ethernet/W5100/%.o ioLibrary/Ethernet/W5100/%.su ioLibrary/Ethernet/W5100/%.cyclo: ../ioLibrary/Ethernet/W5100/%.c ioLibrary/Ethernet/W5100/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G0B0xx -c -I../USB_Device/App -I../USB_Device/Target -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../ioLibrary/Ethernet -I../ioLibrary/Ethernet/W5500 -I../ioLibrary/Internet/MQTT -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-ioLibrary-2f-Ethernet-2f-W5100

clean-ioLibrary-2f-Ethernet-2f-W5100:
	-$(RM) ./ioLibrary/Ethernet/W5100/w5100.cyclo ./ioLibrary/Ethernet/W5100/w5100.d ./ioLibrary/Ethernet/W5100/w5100.o ./ioLibrary/Ethernet/W5100/w5100.su

.PHONY: clean-ioLibrary-2f-Ethernet-2f-W5100

