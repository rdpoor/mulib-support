################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/Users/r/Projects/mulib-support/demos/STM32CubeIDE/NUCLEO-G431RB/mu_platform/mu_platform.c \
/Users/r/Projects/mulib-support/demos/STM32CubeIDE/NUCLEO-G431RB/mu_platform/mu_stddemo.c \
/Users/r/Projects/mulib-support/demos/STM32CubeIDE/NUCLEO-G431RB/mu_platform/mu_time.c 

OBJS += \
./mu_platform/mu_platform.o \
./mu_platform/mu_stddemo.o \
./mu_platform/mu_time.o 

C_DEPS += \
./mu_platform/mu_platform.d \
./mu_platform/mu_stddemo.d \
./mu_platform/mu_time.d 


# Each subdirectory must supply rules for building sources it contributes
mu_platform/mu_platform.o: /Users/r/Projects/mulib-support/demos/STM32CubeIDE/NUCLEO-G431RB/mu_platform/mu_platform.c mu_platform/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../../../../shared/test_stddemo -I../../mu_platform -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"mu_platform/mu_platform.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
mu_platform/mu_stddemo.o: /Users/r/Projects/mulib-support/demos/STM32CubeIDE/NUCLEO-G431RB/mu_platform/mu_stddemo.c mu_platform/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../../../../shared/test_stddemo -I../../mu_platform -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"mu_platform/mu_stddemo.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
mu_platform/mu_time.o: /Users/r/Projects/mulib-support/demos/STM32CubeIDE/NUCLEO-G431RB/mu_platform/mu_time.c mu_platform/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../../../../shared/test_stddemo -I../../mu_platform -I../Core/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"mu_platform/mu_time.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

