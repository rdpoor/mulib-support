################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"/Applications/ti/ccs1031/ccs/tools/compiler/msp430-gcc-9.3.1.11_macos/bin/msp430-elf-gcc-9.3.1" -c -mmcu=msp430fr5994 -mhwmult=f5series -I"/Applications/ti/ccs1031/ccs/ccs_base/msp430/include_gcc" -I"/Users/r/Projects/mulib-support/demos/CodeComposerStudioV10/MSP-EXP430FR5994/mu_platform" -I"/Users/r/Projects/mulib-support/demos/shared/test_stddemo" -I"/Users/r/Projects/mulib-support/mulib" -I"/Users/r/Projects/mulib-support/demos/CodeComposerStudioV10/MSP-EXP430FR5994/test_stddemo_gcc" -I"/Applications/ti/ccs1031/ccs/tools/compiler/msp430-gcc-9.3.1.11_macos/msp430-elf/include" -Og -g -gdwarf-3 -gstrict-dwarf -Wall -mlarge -mcode-region=none -mdata-region=lower -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)"  $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


