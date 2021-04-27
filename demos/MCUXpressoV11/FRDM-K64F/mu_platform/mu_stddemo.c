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
#include "board.h"
#include "mu_platform.h"

// =============================================================================
// Private types and definitions

#define BOARD_LED_GPIO BOARD_LED_RED_GPIO
#define BOARD_LED_GPIO_PIN BOARD_LED_RED_GPIO_PIN

#define BOARD_SW_GPIO BOARD_SW3_GPIO
#define BOARD_SW_PORT BOARD_SW3_PORT
#define BOARD_SW_GPIO_PIN BOARD_SW3_GPIO_PIN
#define BOARD_SW_IRQ BOARD_SW3_IRQ
#define BOARD_SW_IRQ_HANDLER BOARD_SW3_IRQ_HANDLER
#define BOARD_SW_NAME BOARD_SW3_NAME

// =============================================================================
// Private (forward) declarations

// =============================================================================
// Local storage

static mu_stddemo_button_cb s_button_cb;

// =============================================================================
// Public code

void mu_stddemo_init(mu_stddemo_button_cb button_cb) {
  s_button_cb = button_cb;

  gpio_pin_config_t led_config = {
      kGPIO_DigitalOutput,
      0,
  };
  GPIO_PinInit(BOARD_LED_GPIO, BOARD_LED_GPIO_PIN, &led_config);

  gpio_pin_config_t sw_config = {
      kGPIO_DigitalInput,
      0,
  };
  GPIO_PinInit(BOARD_SW_GPIO, BOARD_SW_GPIO_PIN, &sw_config);

  PORT_SetPinInterruptConfig(BOARD_SW_PORT, BOARD_SW_GPIO_PIN, kPORT_InterruptFallingEdge);
  EnableIRQ(BOARD_SW_IRQ);
}

/**
 * @brief Set the demo LED on or off.
 */
void mu_stddemo_led_set(bool on) {
  if (on) {
    GPIO_PortClear(BOARD_LED_GPIO, 1U << BOARD_LED_GPIO_PIN);
  } else {
    GPIO_PortSet(BOARD_LED_GPIO, 1U << BOARD_LED_GPIO_PIN);
  }
}

/**
 * @brief Return true if the demo button is currently pressed.
 *
 * Note that the state of the button can change between the time the button
 * callback is triggered and the button state is read.
 */
bool mu_stddemo_button_is_pressed(void) {
  return GPIO_PinRead(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);
}

// =============================================================================
// Local (static) code

void BOARD_SW_IRQ_HANDLER(void) {
  /* Clear external interrupt flag. */
  GPIO_PortClearInterruptFlags(BOARD_SW_GPIO, 1U << BOARD_SW_GPIO_PIN);
  if (s_button_cb != NULL) {
    s_button_cb(mu_stddemo_button_is_pressed());
  }
  SDK_ISR_EXIT_BARRIER;
}
