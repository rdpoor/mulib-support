################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/Users/r/Projects/mulib-support/demos/shared/test_stddemo/test_stddemo.c 

OBJS += \
./test_stddemo/test_stddemo.o 

C_DEPS += \
./test_stddemo/test_stddemo.d 


# Each subdirectory must supply rules for building sources it contributes
test_stddemo/test_stddemo.o: /Users/r/Projects/mulib-support/demos/shared/test_stddemo/test_stddemo.c test_stddemo/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../../../../shared/test_stddemo -I../../mu_platform -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"test_stddemo/test_stddemo.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

