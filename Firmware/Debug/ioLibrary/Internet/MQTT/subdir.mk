################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ioLibrary/Internet/MQTT/MQTTClient.c \
../ioLibrary/Internet/MQTT/mqtt_interface.c 

OBJS += \
./ioLibrary/Internet/MQTT/MQTTClient.o \
./ioLibrary/Internet/MQTT/mqtt_interface.o 

C_DEPS += \
./ioLibrary/Internet/MQTT/MQTTClient.d \
./ioLibrary/Internet/MQTT/mqtt_interface.d 


# Each subdirectory must supply rules for building sources it contributes
ioLibrary/Internet/MQTT/%.o ioLibrary/Internet/MQTT/%.su ioLibrary/Internet/MQTT/%.cyclo: ../ioLibrary/Internet/MQTT/%.c ioLibrary/Internet/MQTT/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G0B0xx -c -I../USB_Device/App -I../USB_Device/Target -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../ioLibrary/Ethernet -I../ioLibrary/Ethernet/W5500 -I../ioLibrary/Internet/MQTT -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-ioLibrary-2f-Internet-2f-MQTT

clean-ioLibrary-2f-Internet-2f-MQTT:
	-$(RM) ./ioLibrary/Internet/MQTT/MQTTClient.cyclo ./ioLibrary/Internet/MQTT/MQTTClient.d ./ioLibrary/Internet/MQTT/MQTTClient.o ./ioLibrary/Internet/MQTT/MQTTClient.su ./ioLibrary/Internet/MQTT/mqtt_interface.cyclo ./ioLibrary/Internet/MQTT/mqtt_interface.d ./ioLibrary/Internet/MQTT/mqtt_interface.o ./ioLibrary/Internet/MQTT/mqtt_interface.su

.PHONY: clean-ioLibrary-2f-Internet-2f-MQTT

