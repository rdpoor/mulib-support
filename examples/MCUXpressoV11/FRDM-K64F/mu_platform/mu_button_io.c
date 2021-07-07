/**
 * MIT License
 *
 * Copyright (c) 2020 R. D. Poor <rdpoor@gmail.com>
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

#include "mu_button_io.h"

#include "board.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// =============================================================================
// Local types and definitions

#define BOARD_SW_GPIO BOARD_SW3_GPIO
#define BOARD_SW_GPIO_PIN BOARD_SW3_GPIO_PIN
#define BOARD_SW_IRQ_HANDLER BOARD_SW3_IRQ_HANDLER
#define BOARD_SW_PORT BOARD_SW3_PORT

// =============================================================================
// Local storage

static mu_button_io_callback_t s_button_cb;

// =============================================================================
// Local (forward) declarations

// =============================================================================
// Public code

void mu_button_io_init(void) {
  s_button_cb = NULL;

  gpio_pin_config_t sw_config = {
      kGPIO_DigitalInput,
      0,
  };
  GPIO_PinInit(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, &sw_config);

  PORT_SetPinInterruptConfig(BOARD_SW_PORT, BOARD_SW_GPIO_PIN, kPORT_InterruptFallingEdge);
  EnableIRQ(BOARD_SW_IRQ);
}

void mu_button_io_set_callback(mu_button_io_callback_t cb) {
  s_button_cb = cb;
}

bool mu_button_io_get_button(uint8_t button_id) {
  (void)button_id;
  return GPIO_PinRead(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);
}

// =============================================================================
// Local (static) code

void BOARD_SW_IRQ_HANDLER(void) {
  /* Clear external interrupt flag. */
  GPIO_PortClearInterruptFlags(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);
  if (s_button_cb != NULL) {
    s_button_cb(mu_button_io_get_button());
  }
  SDK_ISR_EXIT_BARRIER;
}
