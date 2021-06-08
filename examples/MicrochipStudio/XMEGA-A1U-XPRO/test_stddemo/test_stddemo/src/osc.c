/**
 * \file
 *
 * \brief OSC related functionality implementation.
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
 * \defgroup doc_driver_osc_init OSC Init Driver
 * \ingroup doc_driver_osc
 *
 * \section doc_driver_osc_rev Revision History
 * - v0.0.0.1 Initial Commit
 *
 *@{
 */

#include <osc.h>
#include <ccp.h>

/**
 * \brief Initialize osc interface
 * \return Initialization status.
 */
int8_t OSC_init()
{

	// OSC.XOSCCTRL = OSC_FRQRANGE_04TO2_gc /* 0.4 - 2 MHz */
	//		 | 0 << OSC_X32KLPM_bp /* 32.768 kHz XTAL OSC Low-power Mode: disabled */
	//		 | 0 << OSC_XOSCPWR_bp /* Crystal Oscillator Drive: disabled */
	//		 | OSC_XOSCSEL_EXTCLK_gc; /* External Clock - 6 CLK */

	// OSC.XOSCFAIL = 0 << OSC_PLLFDIF_bp /* PLL Failure Detection Interrupt Flag: disabled */
	//		 | 0 << OSC_XOSCFDIF_bp; /* XOSC Failure Detection Interrupt Flag: disabled */

	// ccp_write_io((void*)&(OSC.XOSCFAIL),0 << OSC_PLLFDEN_bp /* PLL Failure Detection Enable: disabled */
	//		 | 0 << OSC_XOSCFDEN_bp /* XOSC Failure Detection Enable: disabled */);

	// OSC.RC32KCAL = 0; /* Oscillator Calibration Value: 0 */

	// OSC.PLLCTRL = OSC_PLLSRC_RC2M_gc /* 2MHz Internal Oscillator */
	//		 | 1 << OSC_PLLDIV_bp /* PLL divided output: enabled */
	//		 | 1 << OSC_PLLFAC_gp; /* PLL Multiplication Factor: 1 */

	// OSC.CTRL = 0 << OSC_PLLEN_bp /* PLL Enable: disabled */
	//		 | 0 << OSC_XOSCEN_bp /* External Oscillator Enable: disabled */
	//		 | 0 << OSC_RC32KEN_bp /* Internal 32kHz RC Oscillator Enable: disabled */
	//		 | 0 << OSC_RC32MEN_bp /* Internal 32MHz RC Oscillator Enable: disabled */
	//		 | 1 << OSC_RC2MEN_bp; /* Internal 2MHz RC Oscillator Enable: enabled */

	// Wait for the Oscillators to be stable

	while (!(OSC.STATUS & OSC_RC2MRDY_bm)) {
		/* Wait for 2MHz Internal Oscillator to be stable */
	}

	// OSC.DFLLCTRL = OSC_RC32MCREF_RC32K_gc /* Internal 32.768 kHz RC Oscillator */
	//		 | 0 << OSC_RC2MCREF_bp; /* DFLL 2 MHz DFLL Calibration Reference Enable: disabled */

	return 0;
}
