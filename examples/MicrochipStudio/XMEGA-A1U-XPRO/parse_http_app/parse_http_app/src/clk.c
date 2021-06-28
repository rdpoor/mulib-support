/**
 * \file
 *
 * \brief CLK related functionality implementation.
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
 * \defgroup doc_driver_clk_init CLK Init Driver
 * \ingroup doc_driver_clk
 *
 * \section doc_driver_clk_rev Revision History
 * - v0.0.0.1 Initial Commit
 *
 *@{
 */
#include <clk.h>
#include <ccp.h>

/**
 * \brief Initialize clk interface
 * \return Initialization status.
 */
int8_t CLK_init()
{

	// ccp_write_io((void*)&(CLK.CTRL),CLK_SCLKSEL_RC2M_gc /* 2MHz Internal Oscillator */);

	// ccp_write_io((void*)&(CLK.PSCTRL),CLK_PSADIV_1_gc /* Divide by 1 */
	//		 | CLK_PSBCDIV_1_1_gc /* Divide B by 1 and C by 1 */);

	// ccp_write_io((void*)&(CLK.LOCK),0 << CLK_LOCK_bp /* Clock System Lock: disabled */);

	// CLK.RTCCTRL = CLK_RTCSRC_TOSC_gc /* 1.024 kHz from 32.768 kHz crystal oscillator */
	//		 | 0 << CLK_RTCEN_bp; /* RTC Clock Source Enable: disabled */

	// CLK.USBCTRL = 0 << CLK_USBPSDIV_gp /* Prescaler Division Factor: 0 */
	//		 | CLK_USBSRC_PLL_gc /* PLL */
	//		 | 0 << CLK_USBSEN_bp; /* Clock Source Enable: disabled */

	return 0;
}
