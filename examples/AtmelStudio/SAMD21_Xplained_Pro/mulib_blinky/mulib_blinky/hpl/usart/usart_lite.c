
/**
 * \file
 *
 * \brief USART related functionality implementation.
 *
 * Copyright (c) 2017 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */

#include "usart_lite.h"
#include <utils_assert.h>

/**
 * \brief Initialize USART interface
 */
int8_t USART_0_init()
{

	if (!hri_sercomusart_is_syncing(SERCOM3, SERCOM_USART_SYNCBUSY_SWRST)) {
		uint32_t mode = SERCOM_USART_CTRLA_MODE(1);
		if (hri_sercomusart_get_CTRLA_reg(SERCOM3, SERCOM_USART_CTRLA_ENABLE)) {
			hri_sercomusart_clear_CTRLA_ENABLE_bit(SERCOM3);
			hri_sercomusart_wait_for_sync(SERCOM3, SERCOM_USART_SYNCBUSY_ENABLE);
		}
		hri_sercomusart_write_CTRLA_reg(SERCOM3, SERCOM_USART_CTRLA_SWRST | mode);
	}
	hri_sercomusart_wait_for_sync(SERCOM3, SERCOM_USART_SYNCBUSY_SWRST);

	hri_sercomusart_write_CTRLA_reg(
	    SERCOM3,
	    1 << SERCOM_USART_CTRLA_DORD_Pos           /* Data Order: enabled */
	        | 0 << SERCOM_USART_CTRLA_CMODE_Pos    /* Communication Mode: disabled */
	        | 0 << SERCOM_USART_CTRLA_FORM_Pos     /* Frame Format: 0 */
	        | 0 << SERCOM_USART_CTRLA_SAMPA_Pos    /* Sample Adjustment: 0 */
	        | 0 << SERCOM_USART_CTRLA_SAMPR_Pos    /* Sample Rate: 0 */
	        | 0 << SERCOM_USART_CTRLA_IBON_Pos     /* Immediate Buffer Overflow Notification: disabled */
	        | 1 << SERCOM_USART_CTRLA_RUNSTDBY_Pos /* Run In Standby: enabled */
	        | 1 << SERCOM_USART_CTRLA_MODE_Pos);   /* Operating Mode: enabled */

	hri_sercomusart_write_CTRLA_TXPO_bf(SERCOM3, SERCOM3_TXPO);
	hri_sercomusart_write_CTRLA_RXPO_bf(SERCOM3, SERCOM3_RXPO);

	hri_sercomusart_write_CTRLB_reg(
	    SERCOM3,
	    1 << SERCOM_USART_CTRLB_RXEN_Pos         /* Receiver Enable: enabled */
	        | 1 << SERCOM_USART_CTRLB_TXEN_Pos   /* Transmitter Enabl: enabled */
	        | 0 << SERCOM_USART_CTRLB_PMODE_Pos  /* Parity Mode: disabled */
	        | 0 << SERCOM_USART_CTRLB_ENC_Pos    /* Encoding Format: disabled */
	        | 0 << SERCOM_USART_CTRLB_SFDE_Pos   /* Start of Frame Detection Enable: disabled */
	        | 0 << SERCOM_USART_CTRLB_COLDEN_Pos /* Collision Detection Enable: disabled */
	        | 0 << SERCOM_USART_CTRLB_SBMODE_Pos /* Stop Bit Mode: disabled */
	        | 0);                                /* Character Size: 0 */

	hri_sercomusart_write_BAUD_reg(SERCOM3, SERCOM3_BAUD_RATE);

	// hri_sercomusart_write_RXPL_reg(SERCOM3,0); /* Receive Pulse Length: 0 */

	// hri_sercomusart_write_DBGCTRL_reg(SERCOM3,0 << SERCOM_USART_DBGCTRL_DBGSTOP_Pos); /* Debug Stop Mode: disabled */

	hri_sercomusart_write_INTEN_reg(
	    SERCOM3,
	    0 << SERCOM_USART_INTENSET_ERROR_Pos       /* Error Interrupt Enable: disabled */
	        | 0 << SERCOM_USART_INTENSET_RXBRK_Pos /* Receive Break Interrupt Enable: disabled */
	        | 0 << SERCOM_USART_INTENSET_CTSIC_Pos /* Clear to Send Input Change Interrupt Enable: disabled */
	        | 0 << SERCOM_USART_INTENSET_RXS_Pos   /* Receive Start Interrupt Enable: disabled */
	        | 1 << SERCOM_USART_INTENSET_RXC_Pos   /* Receive Complete Interrupt Enable: enabled */
	        | 1 << SERCOM_USART_INTENSET_TXC_Pos   /* Transmit Complete Interrupt Enable: enabled */
	        | 1 << SERCOM_USART_INTENSET_DRE_Pos); /* Data Register Empty Interrupt Enable: enabled */

	hri_sercomusart_write_CTRLA_ENABLE_bit(SERCOM3, 1 << SERCOM_USART_CTRLA_ENABLE_Pos); /* Enable: enabled */

	return 0;
}

/**
 * \brief Write a byte to the SERCOM USART instance
 */
uint8_t USART_0_read_byte()
{
	return hri_sercomusart_read_DATA_reg(SERCOM3);
}

/**
 * \brief Write a byte to the SERCOM USART instance
 */
void USART_0_write_byte(uint8_t data)
{
	hri_sercomusart_write_DATA_reg(SERCOM3, data);
}

/**
 * \brief Check if USART is ready to send next byte
 */
bool USART_0_is_byte_sent()
{
	return hri_sercomusart_get_interrupt_DRE_bit(SERCOM3);
}

/**
 * \brief Check if there is data received by USART
 */
bool USART_0_is_byte_received()
{
	return hri_sercomusart_get_interrupt_RXC_bit(SERCOM3);
}

/**
 * \brief Enable SERCOM module
 */
void USART_0_enable()
{
	hri_sercomusart_set_CTRLA_ENABLE_bit(SERCOM3);
}

/**
 * \brief Disable SERCOM module
 */
void USART_0_disable()
{
	hri_sercomusart_clear_CTRLA_ENABLE_bit(SERCOM3);
}
