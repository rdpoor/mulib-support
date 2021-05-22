################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
mu_platform/mu_platform.obj: /Users/r/Projects/mulib-support/demos/CodeComposerStudioV10/MSP-EXP430FR5994/mu_platform/mu_platform.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/Applications/ti/ccs1031/ccs/tools/compiler/ti-cgt-msp430_20.2.5.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="/Applications/ti/ccs1031/ccs/ccs_base/msp430/include" --include_path="/Users/r/Projects/mulib-support/demos/CodeComposerStudioV10/MSP-EXP430FR5994/test_stddemo_gcc" --include_path="/Applications/ti/ccs1031/ccs/tools/compiler/ti-cgt-msp430_20.2.5.LTS/include" --advice:power=all --advice:hw_config=all --define=__MSP430FR5994__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="mu_platform/$(basename $(<F)).d_raw" --obj_directory="mu_platform" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

mu_platform/mu_stddemo.obj: /Users/r/Projects/mulib-support/demos/CodeComposerStudioV10/MSP-EXP430FR5994/mu_platform/mu_stddemo.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/Applications/ti/ccs1031/ccs/tools/compiler/ti-cgt-msp430_20.2.5.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="/Applications/ti/ccs1031/ccs/ccs_base/msp430/include" --include_path="/Users/r/Projects/mulib-support/demos/CodeComposerStudioV10/MSP-EXP430FR5994/test_stddemo_gcc" --include_path="/Applications/ti/ccs1031/ccs/tools/compiler/ti-cgt-msp430_20.2.5.LTS/include" --advice:power=all --advice:hw_config=all --define=__MSP430FR5994__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="mu_platform/$(basename $(<F)).d_raw" --obj_directory="mu_platform" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

mu_platform/mu_time.obj: /Users/r/Projects/mulib-support/demos/CodeComposerStudioV10/MSP-EXP430FR5994/mu_platform/mu_time.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: MSP430 Compiler'
	"/Applications/ti/ccs1031/ccs/tools/compiler/ti-cgt-msp430_20.2.5.LTS/bin/cl430" -vmspx --data_model=restricted --use_hw_mpy=F5 --include_path="/Applications/ti/ccs1031/ccs/ccs_base/msp430/include" --include_path="/Users/r/Projects/mulib-support/demos/CodeComposerStudioV10/MSP-EXP430FR5994/test_stddemo_gcc" --include_path="/Applications/ti/ccs1031/ccs/tools/compiler/ti-cgt-msp430_20.2.5.LTS/include" --advice:power=all --advice:hw_config=all --define=__MSP430FR5994__ -g --printf_support=minimal --diag_warning=225 --diag_wrap=off --display_error_number --silicon_errata=CPU21 --silicon_errata=CPU22 --silicon_errata=CPU40 --preproc_with_compile --preproc_dependency="mu_platform/$(basename $(<F)).d_raw" --obj_directory="mu_platform" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


