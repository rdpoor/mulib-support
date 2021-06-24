/**
 * MIT License
 *
 * Copyright (c) 2021 R. Dunbar Poor <rdpoor@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

// =============================================================================
// Includes

#include "mu_stddemo.h"
#include <stddef.h>
#include <msp430.h>

// =============================================================================
// Private types and definitions

// =============================================================================
// Private (forward) declarations

/**
 * @brief Initialize GPIO pins.
 *
 * * P1.0 = LED1
 * * P1.1 = LED2
 * * P2.0 = UART TX
 * * P2.1 = UART RX
 * * P5.5 = SW2, Input pull-up
 * * P5.6 = SW1, Input pull-up
 *
 * All others configured as outputs to save power.
 */
static void init_gpios(void);

/**
 * @brief Initialize buttons and interrupt callbacks.
 */
static void init_buttons(void);

// =============================================================================
// Local storage

static mu_stddemo_button_cb s_button1_cb;

static mu_stddemo_button_cb s_button2_cb;

// =============================================================================
// Public code

void mu_button_io_set_callback(mu_stddemo_button_cb button_cb) {
    init_gpios();
    init_buttons();

    // Set up callback for button push
    s_button1_cb = button_cb;
    s_button2_cb = NULL;        // not used in stddemo
}

/**
 * @brief Set the demo LED on or off.
 */
void mu_led_io_set(bool on) {
    if (on) {
        P1OUT |= BIT0;
    } else {
        P1OUT &= ~BIT0;
    }
}

/**
 * @brief Return true if the demo button is currently pressed.
 *
 * Note that the state of the button can change between the time the button
 * callback is triggered and the button state is read.
 */
bool mu_stddemo_button_is_pressed(void) {
    return false;
}

// =============================================================================
// Local (static) code

static void init_gpios(void) {
  // Port output low to save power consumption
  // P1.0 = LED1, Output Low
  // P1.1 = LED2, Output Low
  // P1.2 = Unused, Output Low
  // P1.3 = Unused, Output Low
  // P1.4 = Unused, Output Low
  // P1.5 = Unused, Output Low
  // P1.6 = Unused, Output Low
  // P1.7 = Unused, Output Low
  P1OUT = (0x00);
  P1DIR = (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);

  // P2.0 = UART TX
  // P2.1 = UART RX
  // P2.2 = Unused, Output Low
  // P2.3 = Unused, Output Low
  // P2.4 = Unused, Output Low
  // P2.5 = Unused, Output Low
  // P2.6 = Unused, Output Low
  // P2.7 = Unused, Output Low
  P2OUT = (0x00);
  P2DIR = (BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);
  P2SEL0 &= ~(BIT0 | BIT1);
  P2SEL1 |= (BIT0 | BIT1);

  // P3.0 = Unused, Output Low
  // P3.1 = Unused, Output Low
  // P3.2 = Unused, Output Low
  // P3.3 = Unused, Output Low
  // P3.4 = Unused, Output Low
  // P3.5 = Unused, Output Low
  // P3.6 = Unused, Output Low
  // P3.7 = Unused, Output Low
  P3OUT = (0x00);
  P3DIR = (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);

  // P4.0 = Unused, Output Low
  // P4.1 = Unused, Output Low
  // P4.2 = Unused, Output Low
  // P4.3 = Unused, Output Low
  // P4.4 = Unused, Output Low
  // P4.5 = Unused, Output Low
  // P4.6 = Unused, Output Low
  // P4.7 = Unused, Output Low
  P4OUT = (BIT5);
  P4DIR = (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);

  // P5.0 = Unused, Output Low
  // P5.1 = Unused, Output Low
  // P5.2 = Unused, Output Low
  // P5.3 = Unused, Output Low
  // P5.4 = Unused, Output Low
  // P5.5 = SW2, Input pull-up
  // P5.6 = SW1, Input pull-up
  // P5.7 = Unused, Output Low
  P5OUT = (BIT5 | BIT6);
  P5DIR = (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT7);
  P5REN |= (BIT5 | BIT6);

  // P6.0 = Unused, Output Low
  // P6.1 = Unused, Output Low
  // P6.2 = Unused, Output Low
  // P6.3 = Unused, Output Low
  // P6.4 = Unused, Output Low
  // P6.5 = Unused, Output Low
  // P6.6 = Unused, Output Low
  // P6.7 = Unused, Output Low
  P6OUT = (0x00);
  P6DIR = (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);

  // P7.0 = Unused, Output Low
  // P7.1 = Unused, Output Low
  // P7.2 = Unused, Output Low
  // P7.3 = Unused, Output Low
  // P7.4 = Unused, Output Low
  // P7.5 = Unused, Output Low
  // P7.6 = Unused, Output Low
  // P7.7 = Unused, Output Low
  P7OUT = (0x00);
  P7DIR = (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);

  // P8.0 = Unused, Output Low
  // P8.1 = Unused, Output Low
  // P8.2 = Unused, Output Low
  // P8.3 = Unused, Output Low
  // P8.4 = Unused, Output Low
  // P8.5 = Unused, Output Low
  // P8.6 = Unused, Output Low
  // P8.7 = Unused, Output Low
  P8OUT = (0x00);
  P8DIR = (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);

  // PJ.0 = Unused, Output Low
  // PJ.1 = Unused, Output Low
  // PJ.2 = Unused, Output Low
  // PJ.3 = Unused, Output Low
  // PJ.4 = Unused, Output Low
  // PJ.5 = Unused, Output Low
  // PJ.6 = Unused, Output Low
  // PJ.7 = Unused, Output Low
  PJOUT = (0x00);
  PJDIR = (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7);

  PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                         // to activate previously configured port settings
}

static void init_buttons(void) {
  // Configure SW1 and SW2 for interrupts
 P5IES = (BIT5 | BIT6);                 // Hi/Low edge
 P5IFG = 0;                             // Clear flags
 P5IE = (BIT5 | BIT6);                  // interrupt enabled
}

static void on_button1_press(void) {
  if (s_button1_cb != NULL) {
    s_button1_cb(true);
  }
}

static void on_button2_press(void) {
  if (s_button2_cb != NULL) {
    s_button2_cb(true);
  }
}

// Port 4 interrupt service routine
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=PORT5_VECTOR
__interrupt void Port_5(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(PORT5_VECTOR))) Port_5 (void)
#else
#error Compiler not supported!
#endif
{
    if (P5IFG & BIT6)
    {
        on_button1_press();
        P5IFG &= ~BIT6;                         // Clear IFG
    }
    if (P5IFG & BIT5)
    {
        on_button2_press();
        P5IFG &= ~BIT5;                         // Clear IFG
    }
    __bic_SR_register_on_exit(LPM3_bits);   // Exit LPM3
}
