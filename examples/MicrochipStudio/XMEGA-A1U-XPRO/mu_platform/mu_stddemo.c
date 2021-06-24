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
#include "atmel_start.h"
#include <stddef.h>

// =============================================================================
// Private types and definitions

// =============================================================================
// Private (forward) declarations

// =============================================================================
// Local storage

static mu_stddemo_button_cb s_button_cb;

// =============================================================================
// Public code

void mu_button_io_set_callback(mu_stddemo_button_cb button_cb) {
  s_button_cb = button_cb;
}

/**
 * @brief Set the demo LED on or off.
 */
void mu_led_io_set(bool on) {
  USER_LED_set_level(!on);
}

/**
 * @brief Return true if the demo button is currently pressed.
 *
 * Note that the state of the button can change between the time the button
 * callback is triggered and the button state is read.
 */
bool mu_stddemo_button_is_pressed(void) {
  return USER_BUTTON_get_level();
}

// =============================================================================
// Local (static) code

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void mu_stddemo_on_button_press(void) {
  if (s_button_cb != NULL) {
	  s_button_cb(mu_stddemo_button_is_pressed());
  }
}
