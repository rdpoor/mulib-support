
/**
 * \file
 *
 * \brief USART related functionality declaration.
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

#ifndef _USART_LITE_H_INCLUDED
#define _USART_LITE_H_INCLUDED

#include <compiler.h>
#include <peripheral_clk_config.h>

/**
 * \addtogroup usart USART driver
 *
 * \section usart_rev Revision History
 * - v0.0.0.1 Initial Commit
 *
 *@{
 */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef SERCOM3_RXPO
#define SERCOM3_RXPO 1
#endif

#ifndef SERCOM3_TXPO
#define SERCOM3_TXPO 0
#endif

// Calculate BAUD register value in UART mode

#ifndef SERCOM3_BAUD_RATE
#define SERCOM3_BAUD_RATE 65536 - ((65536 * 16.0f * 115200) / CONF_GCLK_SERCOM3_CORE_FREQUENCY)
#endif

/**
 * \brief Initialize usart interface
 *
 * \return Initialization status.
 */
int8_t USART_0_init();

/**
 * \brief Write a byte to the SERCOM USART instance
 */
uint8_t USART_0_read_byte();

/**
 * \brief Write a byte to the SERCOM USART instance
 */
void USART_0_write_byte(uint8_t data);

/**
 * \brief Check if USART is ready to send next byte
 */
bool USART_0_is_byte_sent();

/**
 * \brief Check if there is data received by USART
 */
bool USART_0_is_byte_received();

/**
 * \brief Enable SERCOM module
 */
void USART_0_enable();

/**
 * \brief Disable SERCOM module
 */
void USART_0_disable();

#ifdef __cplusplus
}
#endif

#endif /* _USART_LITE_H_INCLUDED */
