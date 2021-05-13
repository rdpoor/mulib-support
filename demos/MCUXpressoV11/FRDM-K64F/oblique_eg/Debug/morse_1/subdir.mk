################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
/Users/r/Projects/mulib-support/demos/shared/oblique_eg/oblique_eg.c 

OBJS += \
./oblique_eg/oblique_eg.o 

C_DEPS += \
./oblique_eg/oblique_eg.d 


# Each subdirectory must supply rules for building sources it contributes
oblique_eg/oblique_eg.o: /Users/r/Projects/mulib-support/demos/shared/oblique_eg/oblique_eg.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_MK64FN1M0VLL12 -DCPU_MK64FN1M0VLL12_cm4 -DFRDM_K64F -DFREEDOM -DSDK_DEBUGCONSOLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"/Users/r/Projects/mulib-support/demos/MCUXpressoV11/FRDM-K64F/oblique_eg/board" -I"/Users/r/Projects/mulib-support/mulib" -I"/Users/r/Projects/mulib-support/demos/MCUXpressoV11/FRDM-K64F/oblique_eg/source" -I"/Users/r/Projects/mulib-support/demos/MCUXpressoV11/FRDM-K64F/oblique_eg/utilities" -I"/Users/r/Projects/mulib-support/demos/MCUXpressoV11/FRDM-K64F/oblique_eg/drivers" -I"/Users/r/Projects/mulib-support/demos/MCUXpressoV11/FRDM-K64F/oblique_eg/device" -I"/Users/r/Projects/mulib-support/demos/MCUXpressoV11/FRDM-K64F/oblique_eg/component/uart" -I"/Users/r/Projects/mulib-support/demos/MCUXpressoV11/FRDM-K64F/oblique_eg/component/lists" -I"/Users/r/Projects/mulib-support/demos/MCUXpressoV11/FRDM-K64F/oblique_eg/CMSIS" -I"/Users/r/Projects/mulib-support/demos/MCUXpressoV11/FRDM-K64F/oblique_eg/frdmk64f/driver_examples/gpio/input_interrupt" -I"/Users/r/Projects/mulib-support/demos/MCUXpressoV11/FRDM-K64F/mu_platform" -I"/Users/r/Projects/mulib-support/demos/shared/oblique_eg" -O0 -fno-common -g3 -c -ffunction-sections -fdata-sections -ffreestanding -fno-builtin -fmerge-constants -fmacro-prefix-map="../$(@D)/"=. -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


