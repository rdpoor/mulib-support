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

#include "mu_platform.h"     // must come first
#include "mu_stddemo.h"
#include "stm32g4xx_hal.h"
#include "stm32g4xx_nucleo.h"

// =============================================================================
// Private types and definitions

// =============================================================================
// Private (forward) declarations

// =============================================================================
// Local storage

static mu_stddemo_button_cb s_button_cb;

// =============================================================================
// Public code

void mu_stddemo_init(mu_stddemo_button_cb button_cb) {
  s_button_cb = button_cb;
}

/**
 * @brief Print a formatted message to standard output (usually a serial port).
 */
#define mu_stddemo_printf(fmt, ...) printf(fmt, ##__VA_ARGS__)

/**
 * @brief Set the demo LED on or off.
 */
void mu_stddemo_led_set(bool on) {
      HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

/**
 * @brief Return true if the demo button is currently pressed.
 *
 * Note that the state of the button can change between the time the button
 * callback is triggered and the button state is read.
 */
bool mu_stddemo_button_is_pressed(void) {
      return HAL_GPIO_ReadPin(USER_BUTTON_GPIO_PORT, USER_BUTTON_PIN) == GPIO_PIN_SET;
}

/**
 * @brief Put the processor into low power mode until an interrupt wakes it.
 */
void mu_stddemo_sleep(void) {
  // This implemenation doesn't actually sleep : it busy waits for a button
  // change.
  bool prev_state = mu_stddemo_button_is_pressed();
  while (prev_state == mu_stddemo_button_is_pressed()) {
    // buzz...
  }
}

/**
 * @brief Put the processor into low power mode until the Real Time Clock matches `at`
 * or until an interrupt occurs, whichever comes first.
 */
void mu_stddemo_sleep_until(mu_time_t at) {
  while (mu_time_precedes(mu_time_now(), at)) {
    // buzz
  }
}


// =============================================================================
// Local (static) code

/**
  * @brief EXTI line detection callbacks
  * @param GPIO_Pin: Specifies the pins connected EXTI line
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if ((s_button_cb != NULL) && (GPIO_Pin == USER_BUTTON_PIN)) {
    s_button_cb(mu_stddemo_button_is_pressed());
  }
}
