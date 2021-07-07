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

#include "mu_led_io.h"
#include "atmel_start.h"
#include <stdint.h>
#include <stdbool.h>

// =============================================================================
// Local types and definitions

// =============================================================================
// Local storage

// =============================================================================
// Local (forward) declarations

// =============================================================================
// Public code

void mu_led_io_init(void) {
  gpio_pin_config_t led_config = {
      kGPIO_DigitalOutput,
      0,
  };
  GPIO_PinInit(BOARD_LED_GPIO, BOARD_LED_GPIO_PIN, &led_config);
}

void mu_led_io_set(uint8_t led_id, bool on) {
  if (on) {
    GPIO_PortClear(BOARD_LED_GPIO, 1U << BOARD_LED_GPIO_PIN);
  } else {
    GPIO_PortSet(BOARD_LED_GPIO, 1U << BOARD_LED_GPIO_PIN);
  }
}

bool mu_led_io_get(uint8_t led_id) {
  (void)led_id;
  return GPIO_PinRead(BOARD_LED_GPIO, 1 << BOARD_LED_GPIO_PIN) == 0;
}

// =============================================================================
// Local (static) code
