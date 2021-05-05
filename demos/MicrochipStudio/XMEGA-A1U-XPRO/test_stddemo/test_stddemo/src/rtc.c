/**
 * \file
 *
 * \brief RTC related functionality implementation.
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

/**
 * \defgroup doc_driver_rtc_init RTC Init Driver
 * \ingroup doc_driver_rtc
 *
 * \section doc_driver_rtc_rev Revision History
 * - v0.0.0.1 Initial Commit
 *
 *@{
 */
#include <rtc.h>

/**
 * \brief Initialize rtc interface
 * \return Initialization status.
 */
int8_t RTC_0_init()
{

	while (RTC.STATUS > 0) { /* Wait for register to synchronize */
	}

	RTC.PER = 1; /* Period Register: 1 */

	// RTC.COMP = 0; /* Compare Register: 0 */

	// RTC.CNT = 0; /* 0 */

	// RTC.CTRL = RTC_PRESCALER_OFF_gc; /* RTC Off */

	RTC.INTCTRL = RTC_COMPINTLVL_OFF_gc  /* Interrupt Disabled */
	              | RTC_OVFINTLVL_LO_gc; /* Low Level */

	while (RTC.STATUS > 0) { /* Wait for registers to synchronize before returning from the API */
	}

	return 0;
}
