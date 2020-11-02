#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-same54_xplained_pro.mk)" "nbproject/Makefile-local-same54_xplained_pro.mk"
include nbproject/Makefile-local-same54_xplained_pro.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=same54_xplained_pro
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/mu_thunk_demo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/mu_thunk_demo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

ifeq ($(COMPARE_BUILD), true)
COMPARISON_BUILD=-mafrlcsj
else
COMPARISON_BUILD=
endif

ifdef SUB_IMAGE_ADDRESS

else
SUB_IMAGE_ADDRESS_COMMAND=
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=C:/Users/r/Projects/mulib/src/core/mu_bitvec.c C:/Users/r/Projects/mulib/src/core/mu_list.c C:/Users/r/Projects/mulib/src/core/mu_pstore.c C:/Users/r/Projects/mulib/src/core/mu_queue.c C:/Users/r/Projects/mulib/src/core/mu_sched.c C:/Users/r/Projects/mulib/src/core/mu_spscq.c C:/Users/r/Projects/mulib/src/core/mu_strbuf.c C:/Users/r/Projects/mulib/src/core/mu_substr.c C:/Users/r/Projects/mulib/src/core/mu_thunk.c C:/Users/r/Projects/mulib/src/core/mu_time.c C:/Users/r/Projects/mulib/src/core/mu_version.c ../src/config/same54_xplained_pro/bsp/bsp.c ../src/config/same54_xplained_pro/peripheral/clock/plib_clock.c ../src/config/same54_xplained_pro/peripheral/cmcc/plib_cmcc.c ../src/config/same54_xplained_pro/peripheral/eic/plib_eic.c ../src/config/same54_xplained_pro/peripheral/evsys/plib_evsys.c ../src/config/same54_xplained_pro/peripheral/nvic/plib_nvic.c ../src/config/same54_xplained_pro/peripheral/nvmctrl/plib_nvmctrl.c ../src/config/same54_xplained_pro/peripheral/pm/plib_pm.c ../src/config/same54_xplained_pro/peripheral/port/plib_port.c ../src/config/same54_xplained_pro/peripheral/rtc/plib_rtc_timer.c ../src/config/same54_xplained_pro/peripheral/sercom/usart/plib_sercom2_usart.c ../src/config/same54_xplained_pro/peripheral/supc/plib_supc.c ../src/config/same54_xplained_pro/stdio/xc32_monitor.c ../src/config/same54_xplained_pro/initialization.c ../src/config/same54_xplained_pro/interrupts.c ../src/config/same54_xplained_pro/exceptions.c ../src/config/same54_xplained_pro/startup_xc32.c ../src/config/same54_xplained_pro/libc_syscalls.c C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/button_task.c C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/idle_task.c C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/kbd_task.c C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/led_task.c C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/mu_vm.c C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/screen_redraw_task.c C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/screen_update_task.c C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/mu_thunk_demo.c ../src/main.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/265365281/mu_bitvec.o ${OBJECTDIR}/_ext/265365281/mu_list.o ${OBJECTDIR}/_ext/265365281/mu_pstore.o ${OBJECTDIR}/_ext/265365281/mu_queue.o ${OBJECTDIR}/_ext/265365281/mu_sched.o ${OBJECTDIR}/_ext/265365281/mu_spscq.o ${OBJECTDIR}/_ext/265365281/mu_strbuf.o ${OBJECTDIR}/_ext/265365281/mu_substr.o ${OBJECTDIR}/_ext/265365281/mu_thunk.o ${OBJECTDIR}/_ext/265365281/mu_time.o ${OBJECTDIR}/_ext/265365281/mu_version.o ${OBJECTDIR}/_ext/465512950/bsp.o ${OBJECTDIR}/_ext/199192420/plib_clock.o ${OBJECTDIR}/_ext/560615476/plib_cmcc.o ${OBJECTDIR}/_ext/18086165/plib_eic.o ${OBJECTDIR}/_ext/201341906/plib_evsys.o ${OBJECTDIR}/_ext/560952012/plib_nvic.o ${OBJECTDIR}/_ext/392550042/plib_nvmctrl.o ${OBJECTDIR}/_ext/554773095/plib_pm.o ${OBJECTDIR}/_ext/561005163/plib_port.o ${OBJECTDIR}/_ext/18098999/plib_rtc_timer.o ${OBJECTDIR}/_ext/1547227007/plib_sercom2_usart.o ${OBJECTDIR}/_ext/561100223/plib_supc.o ${OBJECTDIR}/_ext/697067648/xc32_monitor.o ${OBJECTDIR}/_ext/602300742/initialization.o ${OBJECTDIR}/_ext/602300742/interrupts.o ${OBJECTDIR}/_ext/602300742/exceptions.o ${OBJECTDIR}/_ext/602300742/startup_xc32.o ${OBJECTDIR}/_ext/602300742/libc_syscalls.o ${OBJECTDIR}/_ext/914771505/button_task.o ${OBJECTDIR}/_ext/914771505/idle_task.o ${OBJECTDIR}/_ext/914771505/kbd_task.o ${OBJECTDIR}/_ext/914771505/led_task.o ${OBJECTDIR}/_ext/914771505/mu_vm.o ${OBJECTDIR}/_ext/914771505/screen_redraw_task.o ${OBJECTDIR}/_ext/914771505/screen_update_task.o ${OBJECTDIR}/_ext/914771505/mu_thunk_demo.o ${OBJECTDIR}/_ext/1360937237/main.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/265365281/mu_bitvec.o.d ${OBJECTDIR}/_ext/265365281/mu_list.o.d ${OBJECTDIR}/_ext/265365281/mu_pstore.o.d ${OBJECTDIR}/_ext/265365281/mu_queue.o.d ${OBJECTDIR}/_ext/265365281/mu_sched.o.d ${OBJECTDIR}/_ext/265365281/mu_spscq.o.d ${OBJECTDIR}/_ext/265365281/mu_strbuf.o.d ${OBJECTDIR}/_ext/265365281/mu_substr.o.d ${OBJECTDIR}/_ext/265365281/mu_thunk.o.d ${OBJECTDIR}/_ext/265365281/mu_time.o.d ${OBJECTDIR}/_ext/265365281/mu_version.o.d ${OBJECTDIR}/_ext/465512950/bsp.o.d ${OBJECTDIR}/_ext/199192420/plib_clock.o.d ${OBJECTDIR}/_ext/560615476/plib_cmcc.o.d ${OBJECTDIR}/_ext/18086165/plib_eic.o.d ${OBJECTDIR}/_ext/201341906/plib_evsys.o.d ${OBJECTDIR}/_ext/560952012/plib_nvic.o.d ${OBJECTDIR}/_ext/392550042/plib_nvmctrl.o.d ${OBJECTDIR}/_ext/554773095/plib_pm.o.d ${OBJECTDIR}/_ext/561005163/plib_port.o.d ${OBJECTDIR}/_ext/18098999/plib_rtc_timer.o.d ${OBJECTDIR}/_ext/1547227007/plib_sercom2_usart.o.d ${OBJECTDIR}/_ext/561100223/plib_supc.o.d ${OBJECTDIR}/_ext/697067648/xc32_monitor.o.d ${OBJECTDIR}/_ext/602300742/initialization.o.d ${OBJECTDIR}/_ext/602300742/interrupts.o.d ${OBJECTDIR}/_ext/602300742/exceptions.o.d ${OBJECTDIR}/_ext/602300742/startup_xc32.o.d ${OBJECTDIR}/_ext/602300742/libc_syscalls.o.d ${OBJECTDIR}/_ext/914771505/button_task.o.d ${OBJECTDIR}/_ext/914771505/idle_task.o.d ${OBJECTDIR}/_ext/914771505/kbd_task.o.d ${OBJECTDIR}/_ext/914771505/led_task.o.d ${OBJECTDIR}/_ext/914771505/mu_vm.o.d ${OBJECTDIR}/_ext/914771505/screen_redraw_task.o.d ${OBJECTDIR}/_ext/914771505/screen_update_task.o.d ${OBJECTDIR}/_ext/914771505/mu_thunk_demo.o.d ${OBJECTDIR}/_ext/1360937237/main.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/265365281/mu_bitvec.o ${OBJECTDIR}/_ext/265365281/mu_list.o ${OBJECTDIR}/_ext/265365281/mu_pstore.o ${OBJECTDIR}/_ext/265365281/mu_queue.o ${OBJECTDIR}/_ext/265365281/mu_sched.o ${OBJECTDIR}/_ext/265365281/mu_spscq.o ${OBJECTDIR}/_ext/265365281/mu_strbuf.o ${OBJECTDIR}/_ext/265365281/mu_substr.o ${OBJECTDIR}/_ext/265365281/mu_thunk.o ${OBJECTDIR}/_ext/265365281/mu_time.o ${OBJECTDIR}/_ext/265365281/mu_version.o ${OBJECTDIR}/_ext/465512950/bsp.o ${OBJECTDIR}/_ext/199192420/plib_clock.o ${OBJECTDIR}/_ext/560615476/plib_cmcc.o ${OBJECTDIR}/_ext/18086165/plib_eic.o ${OBJECTDIR}/_ext/201341906/plib_evsys.o ${OBJECTDIR}/_ext/560952012/plib_nvic.o ${OBJECTDIR}/_ext/392550042/plib_nvmctrl.o ${OBJECTDIR}/_ext/554773095/plib_pm.o ${OBJECTDIR}/_ext/561005163/plib_port.o ${OBJECTDIR}/_ext/18098999/plib_rtc_timer.o ${OBJECTDIR}/_ext/1547227007/plib_sercom2_usart.o ${OBJECTDIR}/_ext/561100223/plib_supc.o ${OBJECTDIR}/_ext/697067648/xc32_monitor.o ${OBJECTDIR}/_ext/602300742/initialization.o ${OBJECTDIR}/_ext/602300742/interrupts.o ${OBJECTDIR}/_ext/602300742/exceptions.o ${OBJECTDIR}/_ext/602300742/startup_xc32.o ${OBJECTDIR}/_ext/602300742/libc_syscalls.o ${OBJECTDIR}/_ext/914771505/button_task.o ${OBJECTDIR}/_ext/914771505/idle_task.o ${OBJECTDIR}/_ext/914771505/kbd_task.o ${OBJECTDIR}/_ext/914771505/led_task.o ${OBJECTDIR}/_ext/914771505/mu_vm.o ${OBJECTDIR}/_ext/914771505/screen_redraw_task.o ${OBJECTDIR}/_ext/914771505/screen_update_task.o ${OBJECTDIR}/_ext/914771505/mu_thunk_demo.o ${OBJECTDIR}/_ext/1360937237/main.o

# Source Files
SOURCEFILES=C:/Users/r/Projects/mulib/src/core/mu_bitvec.c C:/Users/r/Projects/mulib/src/core/mu_list.c C:/Users/r/Projects/mulib/src/core/mu_pstore.c C:/Users/r/Projects/mulib/src/core/mu_queue.c C:/Users/r/Projects/mulib/src/core/mu_sched.c C:/Users/r/Projects/mulib/src/core/mu_spscq.c C:/Users/r/Projects/mulib/src/core/mu_strbuf.c C:/Users/r/Projects/mulib/src/core/mu_substr.c C:/Users/r/Projects/mulib/src/core/mu_thunk.c C:/Users/r/Projects/mulib/src/core/mu_time.c C:/Users/r/Projects/mulib/src/core/mu_version.c ../src/config/same54_xplained_pro/bsp/bsp.c ../src/config/same54_xplained_pro/peripheral/clock/plib_clock.c ../src/config/same54_xplained_pro/peripheral/cmcc/plib_cmcc.c ../src/config/same54_xplained_pro/peripheral/eic/plib_eic.c ../src/config/same54_xplained_pro/peripheral/evsys/plib_evsys.c ../src/config/same54_xplained_pro/peripheral/nvic/plib_nvic.c ../src/config/same54_xplained_pro/peripheral/nvmctrl/plib_nvmctrl.c ../src/config/same54_xplained_pro/peripheral/pm/plib_pm.c ../src/config/same54_xplained_pro/peripheral/port/plib_port.c ../src/config/same54_xplained_pro/peripheral/rtc/plib_rtc_timer.c ../src/config/same54_xplained_pro/peripheral/sercom/usart/plib_sercom2_usart.c ../src/config/same54_xplained_pro/peripheral/supc/plib_supc.c ../src/config/same54_xplained_pro/stdio/xc32_monitor.c ../src/config/same54_xplained_pro/initialization.c ../src/config/same54_xplained_pro/interrupts.c ../src/config/same54_xplained_pro/exceptions.c ../src/config/same54_xplained_pro/startup_xc32.c ../src/config/same54_xplained_pro/libc_syscalls.c C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/button_task.c C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/idle_task.c C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/kbd_task.c C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/led_task.c C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/mu_vm.c C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/screen_redraw_task.c C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/screen_update_task.c C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/mu_thunk_demo.c ../src/main.c

# Pack Options 
PACK_COMMON_OPTIONS=-I "${CMSIS_DIR}/CMSIS/Core/Include"



CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
ifneq ($(INFORMATION_MESSAGE), )
	@echo $(INFORMATION_MESSAGE)
endif
	${MAKE}  -f nbproject/Makefile-same54_xplained_pro.mk dist/${CND_CONF}/${IMAGE_TYPE}/mu_thunk_demo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=ATSAME54P20A
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/265365281/mu_bitvec.o: C\:/Users/r/Projects/mulib/src/core/mu_bitvec.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265365281" 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_bitvec.o.d 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_bitvec.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/265365281/mu_bitvec.o.d" -o ${OBJECTDIR}/_ext/265365281/mu_bitvec.o C:/Users/r/Projects/mulib/src/core/mu_bitvec.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/265365281/mu_bitvec.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265365281/mu_list.o: C\:/Users/r/Projects/mulib/src/core/mu_list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265365281" 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_list.o.d 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_list.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/265365281/mu_list.o.d" -o ${OBJECTDIR}/_ext/265365281/mu_list.o C:/Users/r/Projects/mulib/src/core/mu_list.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/265365281/mu_list.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265365281/mu_pstore.o: C\:/Users/r/Projects/mulib/src/core/mu_pstore.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265365281" 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_pstore.o.d 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_pstore.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/265365281/mu_pstore.o.d" -o ${OBJECTDIR}/_ext/265365281/mu_pstore.o C:/Users/r/Projects/mulib/src/core/mu_pstore.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/265365281/mu_pstore.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265365281/mu_queue.o: C\:/Users/r/Projects/mulib/src/core/mu_queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265365281" 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_queue.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/265365281/mu_queue.o.d" -o ${OBJECTDIR}/_ext/265365281/mu_queue.o C:/Users/r/Projects/mulib/src/core/mu_queue.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/265365281/mu_queue.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265365281/mu_sched.o: C\:/Users/r/Projects/mulib/src/core/mu_sched.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265365281" 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_sched.o.d 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_sched.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/265365281/mu_sched.o.d" -o ${OBJECTDIR}/_ext/265365281/mu_sched.o C:/Users/r/Projects/mulib/src/core/mu_sched.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/265365281/mu_sched.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265365281/mu_spscq.o: C\:/Users/r/Projects/mulib/src/core/mu_spscq.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265365281" 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_spscq.o.d 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_spscq.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/265365281/mu_spscq.o.d" -o ${OBJECTDIR}/_ext/265365281/mu_spscq.o C:/Users/r/Projects/mulib/src/core/mu_spscq.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/265365281/mu_spscq.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265365281/mu_strbuf.o: C\:/Users/r/Projects/mulib/src/core/mu_strbuf.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265365281" 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_strbuf.o.d 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_strbuf.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/265365281/mu_strbuf.o.d" -o ${OBJECTDIR}/_ext/265365281/mu_strbuf.o C:/Users/r/Projects/mulib/src/core/mu_strbuf.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/265365281/mu_strbuf.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265365281/mu_substr.o: C\:/Users/r/Projects/mulib/src/core/mu_substr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265365281" 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_substr.o.d 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_substr.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/265365281/mu_substr.o.d" -o ${OBJECTDIR}/_ext/265365281/mu_substr.o C:/Users/r/Projects/mulib/src/core/mu_substr.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/265365281/mu_substr.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265365281/mu_thunk.o: C\:/Users/r/Projects/mulib/src/core/mu_thunk.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265365281" 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_thunk.o.d 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_thunk.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/265365281/mu_thunk.o.d" -o ${OBJECTDIR}/_ext/265365281/mu_thunk.o C:/Users/r/Projects/mulib/src/core/mu_thunk.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/265365281/mu_thunk.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265365281/mu_time.o: C\:/Users/r/Projects/mulib/src/core/mu_time.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265365281" 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_time.o.d 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_time.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/265365281/mu_time.o.d" -o ${OBJECTDIR}/_ext/265365281/mu_time.o C:/Users/r/Projects/mulib/src/core/mu_time.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/265365281/mu_time.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265365281/mu_version.o: C\:/Users/r/Projects/mulib/src/core/mu_version.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265365281" 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_version.o.d 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_version.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/265365281/mu_version.o.d" -o ${OBJECTDIR}/_ext/265365281/mu_version.o C:/Users/r/Projects/mulib/src/core/mu_version.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/265365281/mu_version.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/465512950/bsp.o: ../src/config/same54_xplained_pro/bsp/bsp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/465512950" 
	@${RM} ${OBJECTDIR}/_ext/465512950/bsp.o.d 
	@${RM} ${OBJECTDIR}/_ext/465512950/bsp.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/465512950/bsp.o.d" -o ${OBJECTDIR}/_ext/465512950/bsp.o ../src/config/same54_xplained_pro/bsp/bsp.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/465512950/bsp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/199192420/plib_clock.o: ../src/config/same54_xplained_pro/peripheral/clock/plib_clock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/199192420" 
	@${RM} ${OBJECTDIR}/_ext/199192420/plib_clock.o.d 
	@${RM} ${OBJECTDIR}/_ext/199192420/plib_clock.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/199192420/plib_clock.o.d" -o ${OBJECTDIR}/_ext/199192420/plib_clock.o ../src/config/same54_xplained_pro/peripheral/clock/plib_clock.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/199192420/plib_clock.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/560615476/plib_cmcc.o: ../src/config/same54_xplained_pro/peripheral/cmcc/plib_cmcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/560615476" 
	@${RM} ${OBJECTDIR}/_ext/560615476/plib_cmcc.o.d 
	@${RM} ${OBJECTDIR}/_ext/560615476/plib_cmcc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/560615476/plib_cmcc.o.d" -o ${OBJECTDIR}/_ext/560615476/plib_cmcc.o ../src/config/same54_xplained_pro/peripheral/cmcc/plib_cmcc.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/560615476/plib_cmcc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/18086165/plib_eic.o: ../src/config/same54_xplained_pro/peripheral/eic/plib_eic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/18086165" 
	@${RM} ${OBJECTDIR}/_ext/18086165/plib_eic.o.d 
	@${RM} ${OBJECTDIR}/_ext/18086165/plib_eic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/18086165/plib_eic.o.d" -o ${OBJECTDIR}/_ext/18086165/plib_eic.o ../src/config/same54_xplained_pro/peripheral/eic/plib_eic.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/18086165/plib_eic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/201341906/plib_evsys.o: ../src/config/same54_xplained_pro/peripheral/evsys/plib_evsys.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/201341906" 
	@${RM} ${OBJECTDIR}/_ext/201341906/plib_evsys.o.d 
	@${RM} ${OBJECTDIR}/_ext/201341906/plib_evsys.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/201341906/plib_evsys.o.d" -o ${OBJECTDIR}/_ext/201341906/plib_evsys.o ../src/config/same54_xplained_pro/peripheral/evsys/plib_evsys.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/201341906/plib_evsys.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/560952012/plib_nvic.o: ../src/config/same54_xplained_pro/peripheral/nvic/plib_nvic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/560952012" 
	@${RM} ${OBJECTDIR}/_ext/560952012/plib_nvic.o.d 
	@${RM} ${OBJECTDIR}/_ext/560952012/plib_nvic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/560952012/plib_nvic.o.d" -o ${OBJECTDIR}/_ext/560952012/plib_nvic.o ../src/config/same54_xplained_pro/peripheral/nvic/plib_nvic.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/560952012/plib_nvic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/392550042/plib_nvmctrl.o: ../src/config/same54_xplained_pro/peripheral/nvmctrl/plib_nvmctrl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/392550042" 
	@${RM} ${OBJECTDIR}/_ext/392550042/plib_nvmctrl.o.d 
	@${RM} ${OBJECTDIR}/_ext/392550042/plib_nvmctrl.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/392550042/plib_nvmctrl.o.d" -o ${OBJECTDIR}/_ext/392550042/plib_nvmctrl.o ../src/config/same54_xplained_pro/peripheral/nvmctrl/plib_nvmctrl.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/392550042/plib_nvmctrl.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/554773095/plib_pm.o: ../src/config/same54_xplained_pro/peripheral/pm/plib_pm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/554773095" 
	@${RM} ${OBJECTDIR}/_ext/554773095/plib_pm.o.d 
	@${RM} ${OBJECTDIR}/_ext/554773095/plib_pm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/554773095/plib_pm.o.d" -o ${OBJECTDIR}/_ext/554773095/plib_pm.o ../src/config/same54_xplained_pro/peripheral/pm/plib_pm.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/554773095/plib_pm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/561005163/plib_port.o: ../src/config/same54_xplained_pro/peripheral/port/plib_port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/561005163" 
	@${RM} ${OBJECTDIR}/_ext/561005163/plib_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/561005163/plib_port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/561005163/plib_port.o.d" -o ${OBJECTDIR}/_ext/561005163/plib_port.o ../src/config/same54_xplained_pro/peripheral/port/plib_port.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/561005163/plib_port.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/18098999/plib_rtc_timer.o: ../src/config/same54_xplained_pro/peripheral/rtc/plib_rtc_timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/18098999" 
	@${RM} ${OBJECTDIR}/_ext/18098999/plib_rtc_timer.o.d 
	@${RM} ${OBJECTDIR}/_ext/18098999/plib_rtc_timer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/18098999/plib_rtc_timer.o.d" -o ${OBJECTDIR}/_ext/18098999/plib_rtc_timer.o ../src/config/same54_xplained_pro/peripheral/rtc/plib_rtc_timer.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/18098999/plib_rtc_timer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1547227007/plib_sercom2_usart.o: ../src/config/same54_xplained_pro/peripheral/sercom/usart/plib_sercom2_usart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1547227007" 
	@${RM} ${OBJECTDIR}/_ext/1547227007/plib_sercom2_usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1547227007/plib_sercom2_usart.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1547227007/plib_sercom2_usart.o.d" -o ${OBJECTDIR}/_ext/1547227007/plib_sercom2_usart.o ../src/config/same54_xplained_pro/peripheral/sercom/usart/plib_sercom2_usart.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1547227007/plib_sercom2_usart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/561100223/plib_supc.o: ../src/config/same54_xplained_pro/peripheral/supc/plib_supc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/561100223" 
	@${RM} ${OBJECTDIR}/_ext/561100223/plib_supc.o.d 
	@${RM} ${OBJECTDIR}/_ext/561100223/plib_supc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/561100223/plib_supc.o.d" -o ${OBJECTDIR}/_ext/561100223/plib_supc.o ../src/config/same54_xplained_pro/peripheral/supc/plib_supc.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/561100223/plib_supc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/697067648/xc32_monitor.o: ../src/config/same54_xplained_pro/stdio/xc32_monitor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/697067648" 
	@${RM} ${OBJECTDIR}/_ext/697067648/xc32_monitor.o.d 
	@${RM} ${OBJECTDIR}/_ext/697067648/xc32_monitor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/697067648/xc32_monitor.o.d" -o ${OBJECTDIR}/_ext/697067648/xc32_monitor.o ../src/config/same54_xplained_pro/stdio/xc32_monitor.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/697067648/xc32_monitor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/602300742/initialization.o: ../src/config/same54_xplained_pro/initialization.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/602300742" 
	@${RM} ${OBJECTDIR}/_ext/602300742/initialization.o.d 
	@${RM} ${OBJECTDIR}/_ext/602300742/initialization.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/602300742/initialization.o.d" -o ${OBJECTDIR}/_ext/602300742/initialization.o ../src/config/same54_xplained_pro/initialization.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/602300742/initialization.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/602300742/interrupts.o: ../src/config/same54_xplained_pro/interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/602300742" 
	@${RM} ${OBJECTDIR}/_ext/602300742/interrupts.o.d 
	@${RM} ${OBJECTDIR}/_ext/602300742/interrupts.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/602300742/interrupts.o.d" -o ${OBJECTDIR}/_ext/602300742/interrupts.o ../src/config/same54_xplained_pro/interrupts.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/602300742/interrupts.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/602300742/exceptions.o: ../src/config/same54_xplained_pro/exceptions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/602300742" 
	@${RM} ${OBJECTDIR}/_ext/602300742/exceptions.o.d 
	@${RM} ${OBJECTDIR}/_ext/602300742/exceptions.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/602300742/exceptions.o.d" -o ${OBJECTDIR}/_ext/602300742/exceptions.o ../src/config/same54_xplained_pro/exceptions.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/602300742/exceptions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/602300742/startup_xc32.o: ../src/config/same54_xplained_pro/startup_xc32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/602300742" 
	@${RM} ${OBJECTDIR}/_ext/602300742/startup_xc32.o.d 
	@${RM} ${OBJECTDIR}/_ext/602300742/startup_xc32.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/602300742/startup_xc32.o.d" -o ${OBJECTDIR}/_ext/602300742/startup_xc32.o ../src/config/same54_xplained_pro/startup_xc32.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/602300742/startup_xc32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/602300742/libc_syscalls.o: ../src/config/same54_xplained_pro/libc_syscalls.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/602300742" 
	@${RM} ${OBJECTDIR}/_ext/602300742/libc_syscalls.o.d 
	@${RM} ${OBJECTDIR}/_ext/602300742/libc_syscalls.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/602300742/libc_syscalls.o.d" -o ${OBJECTDIR}/_ext/602300742/libc_syscalls.o ../src/config/same54_xplained_pro/libc_syscalls.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/602300742/libc_syscalls.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/914771505/button_task.o: C\:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/button_task.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/914771505" 
	@${RM} ${OBJECTDIR}/_ext/914771505/button_task.o.d 
	@${RM} ${OBJECTDIR}/_ext/914771505/button_task.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/914771505/button_task.o.d" -o ${OBJECTDIR}/_ext/914771505/button_task.o C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/button_task.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/914771505/button_task.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/914771505/idle_task.o: C\:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/idle_task.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/914771505" 
	@${RM} ${OBJECTDIR}/_ext/914771505/idle_task.o.d 
	@${RM} ${OBJECTDIR}/_ext/914771505/idle_task.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/914771505/idle_task.o.d" -o ${OBJECTDIR}/_ext/914771505/idle_task.o C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/idle_task.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/914771505/idle_task.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/914771505/kbd_task.o: C\:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/kbd_task.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/914771505" 
	@${RM} ${OBJECTDIR}/_ext/914771505/kbd_task.o.d 
	@${RM} ${OBJECTDIR}/_ext/914771505/kbd_task.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/914771505/kbd_task.o.d" -o ${OBJECTDIR}/_ext/914771505/kbd_task.o C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/kbd_task.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/914771505/kbd_task.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/914771505/led_task.o: C\:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/led_task.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/914771505" 
	@${RM} ${OBJECTDIR}/_ext/914771505/led_task.o.d 
	@${RM} ${OBJECTDIR}/_ext/914771505/led_task.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/914771505/led_task.o.d" -o ${OBJECTDIR}/_ext/914771505/led_task.o C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/led_task.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/914771505/led_task.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/914771505/mu_vm.o: C\:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/mu_vm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/914771505" 
	@${RM} ${OBJECTDIR}/_ext/914771505/mu_vm.o.d 
	@${RM} ${OBJECTDIR}/_ext/914771505/mu_vm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/914771505/mu_vm.o.d" -o ${OBJECTDIR}/_ext/914771505/mu_vm.o C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/mu_vm.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/914771505/mu_vm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/914771505/screen_redraw_task.o: C\:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/screen_redraw_task.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/914771505" 
	@${RM} ${OBJECTDIR}/_ext/914771505/screen_redraw_task.o.d 
	@${RM} ${OBJECTDIR}/_ext/914771505/screen_redraw_task.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/914771505/screen_redraw_task.o.d" -o ${OBJECTDIR}/_ext/914771505/screen_redraw_task.o C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/screen_redraw_task.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/914771505/screen_redraw_task.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/914771505/screen_update_task.o: C\:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/screen_update_task.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/914771505" 
	@${RM} ${OBJECTDIR}/_ext/914771505/screen_update_task.o.d 
	@${RM} ${OBJECTDIR}/_ext/914771505/screen_update_task.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/914771505/screen_update_task.o.d" -o ${OBJECTDIR}/_ext/914771505/screen_update_task.o C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/screen_update_task.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/914771505/screen_update_task.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/914771505/mu_thunk_demo.o: C\:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/mu_thunk_demo.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/914771505" 
	@${RM} ${OBJECTDIR}/_ext/914771505/mu_thunk_demo.o.d 
	@${RM} ${OBJECTDIR}/_ext/914771505/mu_thunk_demo.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/914771505/mu_thunk_demo.o.d" -o ${OBJECTDIR}/_ext/914771505/mu_thunk_demo.o C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/mu_thunk_demo.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/914771505/mu_thunk_demo.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
else
${OBJECTDIR}/_ext/265365281/mu_bitvec.o: C\:/Users/r/Projects/mulib/src/core/mu_bitvec.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265365281" 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_bitvec.o.d 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_bitvec.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/265365281/mu_bitvec.o.d" -o ${OBJECTDIR}/_ext/265365281/mu_bitvec.o C:/Users/r/Projects/mulib/src/core/mu_bitvec.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/265365281/mu_bitvec.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265365281/mu_list.o: C\:/Users/r/Projects/mulib/src/core/mu_list.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265365281" 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_list.o.d 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_list.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/265365281/mu_list.o.d" -o ${OBJECTDIR}/_ext/265365281/mu_list.o C:/Users/r/Projects/mulib/src/core/mu_list.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/265365281/mu_list.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265365281/mu_pstore.o: C\:/Users/r/Projects/mulib/src/core/mu_pstore.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265365281" 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_pstore.o.d 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_pstore.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/265365281/mu_pstore.o.d" -o ${OBJECTDIR}/_ext/265365281/mu_pstore.o C:/Users/r/Projects/mulib/src/core/mu_pstore.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/265365281/mu_pstore.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265365281/mu_queue.o: C\:/Users/r/Projects/mulib/src/core/mu_queue.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265365281" 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_queue.o.d 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_queue.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/265365281/mu_queue.o.d" -o ${OBJECTDIR}/_ext/265365281/mu_queue.o C:/Users/r/Projects/mulib/src/core/mu_queue.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/265365281/mu_queue.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265365281/mu_sched.o: C\:/Users/r/Projects/mulib/src/core/mu_sched.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265365281" 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_sched.o.d 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_sched.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/265365281/mu_sched.o.d" -o ${OBJECTDIR}/_ext/265365281/mu_sched.o C:/Users/r/Projects/mulib/src/core/mu_sched.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/265365281/mu_sched.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265365281/mu_spscq.o: C\:/Users/r/Projects/mulib/src/core/mu_spscq.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265365281" 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_spscq.o.d 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_spscq.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/265365281/mu_spscq.o.d" -o ${OBJECTDIR}/_ext/265365281/mu_spscq.o C:/Users/r/Projects/mulib/src/core/mu_spscq.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/265365281/mu_spscq.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265365281/mu_strbuf.o: C\:/Users/r/Projects/mulib/src/core/mu_strbuf.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265365281" 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_strbuf.o.d 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_strbuf.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/265365281/mu_strbuf.o.d" -o ${OBJECTDIR}/_ext/265365281/mu_strbuf.o C:/Users/r/Projects/mulib/src/core/mu_strbuf.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/265365281/mu_strbuf.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265365281/mu_substr.o: C\:/Users/r/Projects/mulib/src/core/mu_substr.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265365281" 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_substr.o.d 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_substr.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/265365281/mu_substr.o.d" -o ${OBJECTDIR}/_ext/265365281/mu_substr.o C:/Users/r/Projects/mulib/src/core/mu_substr.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/265365281/mu_substr.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265365281/mu_thunk.o: C\:/Users/r/Projects/mulib/src/core/mu_thunk.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265365281" 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_thunk.o.d 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_thunk.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/265365281/mu_thunk.o.d" -o ${OBJECTDIR}/_ext/265365281/mu_thunk.o C:/Users/r/Projects/mulib/src/core/mu_thunk.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/265365281/mu_thunk.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265365281/mu_time.o: C\:/Users/r/Projects/mulib/src/core/mu_time.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265365281" 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_time.o.d 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_time.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/265365281/mu_time.o.d" -o ${OBJECTDIR}/_ext/265365281/mu_time.o C:/Users/r/Projects/mulib/src/core/mu_time.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/265365281/mu_time.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/265365281/mu_version.o: C\:/Users/r/Projects/mulib/src/core/mu_version.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/265365281" 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_version.o.d 
	@${RM} ${OBJECTDIR}/_ext/265365281/mu_version.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/265365281/mu_version.o.d" -o ${OBJECTDIR}/_ext/265365281/mu_version.o C:/Users/r/Projects/mulib/src/core/mu_version.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/265365281/mu_version.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/465512950/bsp.o: ../src/config/same54_xplained_pro/bsp/bsp.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/465512950" 
	@${RM} ${OBJECTDIR}/_ext/465512950/bsp.o.d 
	@${RM} ${OBJECTDIR}/_ext/465512950/bsp.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/465512950/bsp.o.d" -o ${OBJECTDIR}/_ext/465512950/bsp.o ../src/config/same54_xplained_pro/bsp/bsp.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/465512950/bsp.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/199192420/plib_clock.o: ../src/config/same54_xplained_pro/peripheral/clock/plib_clock.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/199192420" 
	@${RM} ${OBJECTDIR}/_ext/199192420/plib_clock.o.d 
	@${RM} ${OBJECTDIR}/_ext/199192420/plib_clock.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/199192420/plib_clock.o.d" -o ${OBJECTDIR}/_ext/199192420/plib_clock.o ../src/config/same54_xplained_pro/peripheral/clock/plib_clock.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/199192420/plib_clock.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/560615476/plib_cmcc.o: ../src/config/same54_xplained_pro/peripheral/cmcc/plib_cmcc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/560615476" 
	@${RM} ${OBJECTDIR}/_ext/560615476/plib_cmcc.o.d 
	@${RM} ${OBJECTDIR}/_ext/560615476/plib_cmcc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/560615476/plib_cmcc.o.d" -o ${OBJECTDIR}/_ext/560615476/plib_cmcc.o ../src/config/same54_xplained_pro/peripheral/cmcc/plib_cmcc.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/560615476/plib_cmcc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/18086165/plib_eic.o: ../src/config/same54_xplained_pro/peripheral/eic/plib_eic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/18086165" 
	@${RM} ${OBJECTDIR}/_ext/18086165/plib_eic.o.d 
	@${RM} ${OBJECTDIR}/_ext/18086165/plib_eic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/18086165/plib_eic.o.d" -o ${OBJECTDIR}/_ext/18086165/plib_eic.o ../src/config/same54_xplained_pro/peripheral/eic/plib_eic.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/18086165/plib_eic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/201341906/plib_evsys.o: ../src/config/same54_xplained_pro/peripheral/evsys/plib_evsys.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/201341906" 
	@${RM} ${OBJECTDIR}/_ext/201341906/plib_evsys.o.d 
	@${RM} ${OBJECTDIR}/_ext/201341906/plib_evsys.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/201341906/plib_evsys.o.d" -o ${OBJECTDIR}/_ext/201341906/plib_evsys.o ../src/config/same54_xplained_pro/peripheral/evsys/plib_evsys.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/201341906/plib_evsys.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/560952012/plib_nvic.o: ../src/config/same54_xplained_pro/peripheral/nvic/plib_nvic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/560952012" 
	@${RM} ${OBJECTDIR}/_ext/560952012/plib_nvic.o.d 
	@${RM} ${OBJECTDIR}/_ext/560952012/plib_nvic.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/560952012/plib_nvic.o.d" -o ${OBJECTDIR}/_ext/560952012/plib_nvic.o ../src/config/same54_xplained_pro/peripheral/nvic/plib_nvic.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/560952012/plib_nvic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/392550042/plib_nvmctrl.o: ../src/config/same54_xplained_pro/peripheral/nvmctrl/plib_nvmctrl.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/392550042" 
	@${RM} ${OBJECTDIR}/_ext/392550042/plib_nvmctrl.o.d 
	@${RM} ${OBJECTDIR}/_ext/392550042/plib_nvmctrl.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/392550042/plib_nvmctrl.o.d" -o ${OBJECTDIR}/_ext/392550042/plib_nvmctrl.o ../src/config/same54_xplained_pro/peripheral/nvmctrl/plib_nvmctrl.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/392550042/plib_nvmctrl.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/554773095/plib_pm.o: ../src/config/same54_xplained_pro/peripheral/pm/plib_pm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/554773095" 
	@${RM} ${OBJECTDIR}/_ext/554773095/plib_pm.o.d 
	@${RM} ${OBJECTDIR}/_ext/554773095/plib_pm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/554773095/plib_pm.o.d" -o ${OBJECTDIR}/_ext/554773095/plib_pm.o ../src/config/same54_xplained_pro/peripheral/pm/plib_pm.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/554773095/plib_pm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/561005163/plib_port.o: ../src/config/same54_xplained_pro/peripheral/port/plib_port.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/561005163" 
	@${RM} ${OBJECTDIR}/_ext/561005163/plib_port.o.d 
	@${RM} ${OBJECTDIR}/_ext/561005163/plib_port.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/561005163/plib_port.o.d" -o ${OBJECTDIR}/_ext/561005163/plib_port.o ../src/config/same54_xplained_pro/peripheral/port/plib_port.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/561005163/plib_port.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/18098999/plib_rtc_timer.o: ../src/config/same54_xplained_pro/peripheral/rtc/plib_rtc_timer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/18098999" 
	@${RM} ${OBJECTDIR}/_ext/18098999/plib_rtc_timer.o.d 
	@${RM} ${OBJECTDIR}/_ext/18098999/plib_rtc_timer.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/18098999/plib_rtc_timer.o.d" -o ${OBJECTDIR}/_ext/18098999/plib_rtc_timer.o ../src/config/same54_xplained_pro/peripheral/rtc/plib_rtc_timer.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/18098999/plib_rtc_timer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1547227007/plib_sercom2_usart.o: ../src/config/same54_xplained_pro/peripheral/sercom/usart/plib_sercom2_usart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1547227007" 
	@${RM} ${OBJECTDIR}/_ext/1547227007/plib_sercom2_usart.o.d 
	@${RM} ${OBJECTDIR}/_ext/1547227007/plib_sercom2_usart.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1547227007/plib_sercom2_usart.o.d" -o ${OBJECTDIR}/_ext/1547227007/plib_sercom2_usart.o ../src/config/same54_xplained_pro/peripheral/sercom/usart/plib_sercom2_usart.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1547227007/plib_sercom2_usart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/561100223/plib_supc.o: ../src/config/same54_xplained_pro/peripheral/supc/plib_supc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/561100223" 
	@${RM} ${OBJECTDIR}/_ext/561100223/plib_supc.o.d 
	@${RM} ${OBJECTDIR}/_ext/561100223/plib_supc.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/561100223/plib_supc.o.d" -o ${OBJECTDIR}/_ext/561100223/plib_supc.o ../src/config/same54_xplained_pro/peripheral/supc/plib_supc.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/561100223/plib_supc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/697067648/xc32_monitor.o: ../src/config/same54_xplained_pro/stdio/xc32_monitor.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/697067648" 
	@${RM} ${OBJECTDIR}/_ext/697067648/xc32_monitor.o.d 
	@${RM} ${OBJECTDIR}/_ext/697067648/xc32_monitor.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/697067648/xc32_monitor.o.d" -o ${OBJECTDIR}/_ext/697067648/xc32_monitor.o ../src/config/same54_xplained_pro/stdio/xc32_monitor.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/697067648/xc32_monitor.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/602300742/initialization.o: ../src/config/same54_xplained_pro/initialization.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/602300742" 
	@${RM} ${OBJECTDIR}/_ext/602300742/initialization.o.d 
	@${RM} ${OBJECTDIR}/_ext/602300742/initialization.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/602300742/initialization.o.d" -o ${OBJECTDIR}/_ext/602300742/initialization.o ../src/config/same54_xplained_pro/initialization.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/602300742/initialization.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/602300742/interrupts.o: ../src/config/same54_xplained_pro/interrupts.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/602300742" 
	@${RM} ${OBJECTDIR}/_ext/602300742/interrupts.o.d 
	@${RM} ${OBJECTDIR}/_ext/602300742/interrupts.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/602300742/interrupts.o.d" -o ${OBJECTDIR}/_ext/602300742/interrupts.o ../src/config/same54_xplained_pro/interrupts.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/602300742/interrupts.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/602300742/exceptions.o: ../src/config/same54_xplained_pro/exceptions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/602300742" 
	@${RM} ${OBJECTDIR}/_ext/602300742/exceptions.o.d 
	@${RM} ${OBJECTDIR}/_ext/602300742/exceptions.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/602300742/exceptions.o.d" -o ${OBJECTDIR}/_ext/602300742/exceptions.o ../src/config/same54_xplained_pro/exceptions.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/602300742/exceptions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/602300742/startup_xc32.o: ../src/config/same54_xplained_pro/startup_xc32.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/602300742" 
	@${RM} ${OBJECTDIR}/_ext/602300742/startup_xc32.o.d 
	@${RM} ${OBJECTDIR}/_ext/602300742/startup_xc32.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/602300742/startup_xc32.o.d" -o ${OBJECTDIR}/_ext/602300742/startup_xc32.o ../src/config/same54_xplained_pro/startup_xc32.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/602300742/startup_xc32.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/602300742/libc_syscalls.o: ../src/config/same54_xplained_pro/libc_syscalls.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/602300742" 
	@${RM} ${OBJECTDIR}/_ext/602300742/libc_syscalls.o.d 
	@${RM} ${OBJECTDIR}/_ext/602300742/libc_syscalls.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/602300742/libc_syscalls.o.d" -o ${OBJECTDIR}/_ext/602300742/libc_syscalls.o ../src/config/same54_xplained_pro/libc_syscalls.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/602300742/libc_syscalls.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/914771505/button_task.o: C\:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/button_task.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/914771505" 
	@${RM} ${OBJECTDIR}/_ext/914771505/button_task.o.d 
	@${RM} ${OBJECTDIR}/_ext/914771505/button_task.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/914771505/button_task.o.d" -o ${OBJECTDIR}/_ext/914771505/button_task.o C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/button_task.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/914771505/button_task.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/914771505/idle_task.o: C\:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/idle_task.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/914771505" 
	@${RM} ${OBJECTDIR}/_ext/914771505/idle_task.o.d 
	@${RM} ${OBJECTDIR}/_ext/914771505/idle_task.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/914771505/idle_task.o.d" -o ${OBJECTDIR}/_ext/914771505/idle_task.o C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/idle_task.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/914771505/idle_task.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/914771505/kbd_task.o: C\:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/kbd_task.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/914771505" 
	@${RM} ${OBJECTDIR}/_ext/914771505/kbd_task.o.d 
	@${RM} ${OBJECTDIR}/_ext/914771505/kbd_task.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/914771505/kbd_task.o.d" -o ${OBJECTDIR}/_ext/914771505/kbd_task.o C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/kbd_task.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/914771505/kbd_task.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/914771505/led_task.o: C\:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/led_task.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/914771505" 
	@${RM} ${OBJECTDIR}/_ext/914771505/led_task.o.d 
	@${RM} ${OBJECTDIR}/_ext/914771505/led_task.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/914771505/led_task.o.d" -o ${OBJECTDIR}/_ext/914771505/led_task.o C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/led_task.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/914771505/led_task.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/914771505/mu_vm.o: C\:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/mu_vm.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/914771505" 
	@${RM} ${OBJECTDIR}/_ext/914771505/mu_vm.o.d 
	@${RM} ${OBJECTDIR}/_ext/914771505/mu_vm.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/914771505/mu_vm.o.d" -o ${OBJECTDIR}/_ext/914771505/mu_vm.o C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/mu_vm.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/914771505/mu_vm.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/914771505/screen_redraw_task.o: C\:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/screen_redraw_task.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/914771505" 
	@${RM} ${OBJECTDIR}/_ext/914771505/screen_redraw_task.o.d 
	@${RM} ${OBJECTDIR}/_ext/914771505/screen_redraw_task.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/914771505/screen_redraw_task.o.d" -o ${OBJECTDIR}/_ext/914771505/screen_redraw_task.o C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/screen_redraw_task.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/914771505/screen_redraw_task.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/914771505/screen_update_task.o: C\:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/screen_update_task.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/914771505" 
	@${RM} ${OBJECTDIR}/_ext/914771505/screen_update_task.o.d 
	@${RM} ${OBJECTDIR}/_ext/914771505/screen_update_task.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/914771505/screen_update_task.o.d" -o ${OBJECTDIR}/_ext/914771505/screen_update_task.o C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/screen_update_task.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/914771505/screen_update_task.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/914771505/mu_thunk_demo.o: C\:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/mu_thunk_demo.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/914771505" 
	@${RM} ${OBJECTDIR}/_ext/914771505/mu_thunk_demo.o.d 
	@${RM} ${OBJECTDIR}/_ext/914771505/mu_thunk_demo.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/914771505/mu_thunk_demo.o.d" -o ${OBJECTDIR}/_ext/914771505/mu_thunk_demo.o C:/Users/r/Projects/mulib/examples/MPLAB/SAME54_Xplained_Pro/mu_thunk_demo/firmware/src/mu_thunk_demo.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/914771505/mu_thunk_demo.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} "${OBJECTDIR}/_ext/1360937237" 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -I"../../../../../../src" -ffunction-sections -fdata-sections -O1 -I"../src" -I"../src/config/same54_xplained_pro" -I"../src/packs/ATSAME54P20A_DFP" -I"../src/packs/CMSIS/CMSIS/Core/Include" -I"../src/packs/CMSIS/" -Werror -Wall -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c    -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -mdfp="${DFP_DIR}" ${PACK_COMMON_OPTIONS} 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/mu_thunk_demo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE) -g   -mprocessor=$(MP_PROCESSOR_OPTION) -mno-device-startup-code -o dist/${CND_CONF}/${IMAGE_TYPE}/mu_thunk_demo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,-D=__DEBUG_D,--defsym=_min_heap_size=512,--gc-sections,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -mdfp="${DFP_DIR}"
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/mu_thunk_demo.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION) -mno-device-startup-code -o dist/${CND_CONF}/${IMAGE_TYPE}/mu_thunk_demo.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -DXPRJ_same54_xplained_pro=$(CND_CONF)    $(COMPARISON_BUILD)  -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=512,--gc-sections,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map",--memorysummary,dist/${CND_CONF}/${IMAGE_TYPE}/memoryfile.xml -mdfp="${DFP_DIR}"
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/mu_thunk_demo.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/same54_xplained_pro
	${RM} -r dist/same54_xplained_pro

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
