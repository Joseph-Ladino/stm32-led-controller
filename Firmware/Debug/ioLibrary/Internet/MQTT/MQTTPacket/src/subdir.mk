################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTConnectClient.c \
../ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTConnectServer.c \
../ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTDeserializePublish.c \
../ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTFormat.c \
../ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTPacket.c \
../ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTSerializePublish.c \
../ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTSubscribeClient.c \
../ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTSubscribeServer.c \
../ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeClient.c \
../ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeServer.c 

OBJS += \
./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTConnectClient.o \
./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTConnectServer.o \
./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTDeserializePublish.o \
./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTFormat.o \
./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTPacket.o \
./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTSerializePublish.o \
./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTSubscribeClient.o \
./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTSubscribeServer.o \
./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeClient.o \
./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeServer.o 

C_DEPS += \
./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTConnectClient.d \
./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTConnectServer.d \
./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTDeserializePublish.d \
./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTFormat.d \
./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTPacket.d \
./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTSerializePublish.d \
./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTSubscribeClient.d \
./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTSubscribeServer.d \
./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeClient.d \
./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeServer.d 


# Each subdirectory must supply rules for building sources it contributes
ioLibrary/Internet/MQTT/MQTTPacket/src/%.o ioLibrary/Internet/MQTT/MQTTPacket/src/%.su ioLibrary/Internet/MQTT/MQTTPacket/src/%.cyclo: ../ioLibrary/Internet/MQTT/MQTTPacket/src/%.c ioLibrary/Internet/MQTT/MQTTPacket/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m0plus -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G0B0xx -c -I../USB_Device/App -I../USB_Device/Target -I../Core/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc -I../Drivers/STM32G0xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Drivers/CMSIS/Device/ST/STM32G0xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../ioLibrary/Ethernet -I../ioLibrary/Ethernet/W5500 -I../ioLibrary/Internet/MQTT -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-ioLibrary-2f-Internet-2f-MQTT-2f-MQTTPacket-2f-src

clean-ioLibrary-2f-Internet-2f-MQTT-2f-MQTTPacket-2f-src:
	-$(RM) ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTConnectClient.cyclo ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTConnectClient.d ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTConnectClient.o ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTConnectClient.su ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTConnectServer.cyclo ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTConnectServer.d ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTConnectServer.o ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTConnectServer.su ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTDeserializePublish.cyclo ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTDeserializePublish.d ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTDeserializePublish.o ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTDeserializePublish.su ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTFormat.cyclo ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTFormat.d ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTFormat.o ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTFormat.su ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTPacket.cyclo ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTPacket.d ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTPacket.o ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTPacket.su ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTSerializePublish.cyclo ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTSerializePublish.d ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTSerializePublish.o ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTSerializePublish.su ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTSubscribeClient.cyclo ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTSubscribeClient.d ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTSubscribeClient.o ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTSubscribeClient.su ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTSubscribeServer.cyclo ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTSubscribeServer.d ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTSubscribeServer.o ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTSubscribeServer.su ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeClient.cyclo ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeClient.d ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeClient.o ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeClient.su ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeServer.cyclo ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeServer.d ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeServer.o ./ioLibrary/Internet/MQTT/MQTTPacket/src/MQTTUnsubscribeServer.su

.PHONY: clean-ioLibrary-2f-Internet-2f-MQTT-2f-MQTTPacket-2f-src

