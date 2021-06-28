/**
 * \file
 *
 * \brief Driver initialization.
 *
 (c) 2020 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms,you may use this software and
    any derivatives exclusively with Microchip products.It is your responsibility
    to comply with third party license terms applicable to your use of third party
    software (including open source software) that may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 */

/*
 * Code generated by START.
 *
 * This file will be overwritten when reconfiguring your START project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <system.h>

void TIMER_0_initialization(void)
{

	TIMER_0_init();
}

/* configure pins and initialize registers */
void SLEEP_initialization(void)
{
	SLEEP_init();
}

/* Configure pins and initialize registers */
void USART_0_initialization(void)
{

	// Set pin direction to input
	PE2_set_dir(PORT_DIR_IN);

	PE2_set_output_pull_mode(
	    // <y> Output and Pull Configuration
	    // <id> pad_output_pull_config
	    // <PORT_CONFIGURATION_TOTEM"> Totem-pole
	    // <PORT_CONFIGURATION_BUSKEEPER"> Totem-pole with bus-keeper
	    // <PORT_CONFIGURATION_PULLDOWN"> Pull-down
	    // <PORT_CONFIGURATION_PULLUP"> Pull-up
	    // <PORT_CONFIGURATION_WIREDOR"> Wired-OR
	    // <PORT_CONFIGURATION_WIREDAND"> Wired-AND
	    // <PORT_CONFIGURATION_WIREDORPULL"> Wired-OR with pull-down
	    // <PORT_CONFIGURATION_WIREDANDPULL"> Wired-AND with pull-up
	    PORT_CONFIGURATION_TOTEM);

	// Set pin direction to output
	PE3_set_dir(PORT_DIR_OUT);

	PE3_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    false);

	USART_0_init();
}

/**
 * \brief System initialization
 */
void system_init()
{
	mcu_init();

	/* PORT setting on PQ2 */

	USER_BUTTON_set_dir(
	    // <y> Pin direction
	    // <id> pad_dir
	    // <PORT_DIR_OFF"> Off
	    // <PORT_DIR_IN"> In
	    // <PORT_DIR_OUT"> Out
	    PORT_DIR_IN);

	USER_BUTTON_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    true);

	USER_BUTTON_set_output_pull_mode(
	    // <y> Output and Pull Configuration
	    // <id> pad_output_pull_config
	    // <PORT_CONFIGURATION_TOTEM"> Totem-pole
	    // <PORT_CONFIGURATION_BUSKEEPER"> Totem-pole with bus-keeper
	    // <PORT_CONFIGURATION_PULLDOWN"> Pull-down
	    // <PORT_CONFIGURATION_PULLUP"> Pull-up
	    // <PORT_CONFIGURATION_WIREDOR"> Wired-OR
	    // <PORT_CONFIGURATION_WIREDAND"> Wired-AND
	    // <PORT_CONFIGURATION_WIREDORPULL"> Wired-OR with pull-down
	    // <PORT_CONFIGURATION_WIREDANDPULL"> Wired-AND with pull-up
	    PORT_CONFIGURATION_PULLUP);

	USER_BUTTON_set_isc(
	    // <y> Pin Input/Sense Configuration
	    // <id> pad_isc
	    // <PORT_ISC_BOTHEDGES_gc"> Sense Both Edges
	    // <PORT_ISC_RISING_gc"> Sense Rising Edge
	    // <PORT_ISC_FALLING_gc"> Sense Falling Edge
	    // <PORT_ISC_INPUT_DISABLE_gc"> Digital Input Buffer disabled
	    // <PORT_ISC_LEVEL_gc"> Sense low Level
	    PORT_ISC_FALLING_gc);

	USER_BUTTON_int_level(
	    // <y> port vector 0 int level
	    // <id> pad_int0lvl
	    // <PORT_INT0LVL_OFF_gc"> Interrupt Disabled
	    // <PORT_INT0LVL_LO_gc"> Low Level
	    // <PORT_INT0LVL_MED_gc"> Medium Level
	    // <PORT_INT0LVL_HI_gc"> High Level
	    PORT_INT0LVL_LO_gc);

	USER_BUTTON_int_level(
	    // <y> port vector 1 int level
	    // <id> pad_int1lvl
	    // <PORT_INT1LVL_OFF_gc"> Interrupt Disabled
	    // <PORT_INT1LVL_LO_gc"> Low Level
	    // <PORT_INT1LVL_MED_gc"> Medium Level
	    // <PORT_INT1LVL_HI_gc"> High Level
	    PORT_INT1LVL_OFF_gc);

	USER_BUTTON_int0_mask(
	    // <o> port vector 0 int mask
	    // <id> pad_int0msk
	    // <0=> 0
	    // <1=> 1
	    // <2=> 2
	    // <3=> 3
	    // <4=> 4
	    // <5=> 5
	    // <6=> 6
	    // <7=> 7
	    // <8=> 8
	    // <9=> 9
	    // <10=> 10
	    // <11=> 11
	    // <12=> 12
	    // <13=> 13
	    // <14=> 14
	    0x4);

	USER_BUTTON_int1_mask(
	    // <o> port vector 1 int mask
	    // <id> pad_int1msk
	    // <0=> 0
	    // <1=> 1
	    // <2=> 2
	    // <3=> 3
	    // <4=> 4
	    // <5=> 5
	    // <6=> 6
	    // <7=> 7
	    // <8=> 8
	    // <9=> 9
	    // <10=> 10
	    // <11=> 11
	    // <12=> 12
	    // <13=> 13
	    // <14=> 14
	    0x0);

	/* PORT setting on PQ3 */

	// Set pin direction to output
	USER_LED_set_dir(PORT_DIR_OUT);

	USER_LED_set_level(
	    // <y> Initial level
	    // <id> pad_initial_level
	    // <false"> Low
	    // <true"> High
	    true);

	TIMER_0_initialization();

	OSC_init();

	CLK_init();

	PMIC_init();

	SLEEP_initialization();

	USART_0_initialization();
}
