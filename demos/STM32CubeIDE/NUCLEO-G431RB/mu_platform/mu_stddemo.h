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

#ifndef _MU_STDDEMO_H_
#define _MU_STDDEMO_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// Includes

#include <stdbool.h>
#include "mu_time.h"

// =============================================================================
// Types and definitions

/**
 * @brief Signature for a button callback.
 *
 * This is a user-supplied function that gets called at interrupt level when the
 * button chagnes state.
 *
 * @param button_state True if the button is pressed at the time of interrupt.
 */
typedef void (*mu_stddemo_button_cb)(bool button_state);

// =============================================================================
// Functon declarations (public)

/**
 * @brief Initialize the mu_stddemo_support system.
 *
 * @param button_cb Function to call from interrupt level when the user button
 * is pressed.  Set to NULL to inhibit callbacks.
 */
void mu_stddemo_init(mu_stddemo_button_cb button_cb);

/**
 * @brief Print a formatted message to standard output (usually a serial port).
 */
#define mu_stddemo_printf(fmt, ...) printf(fmt, ##__VA_ARGS__)

/**
 * @brief Set the demo LED on or off.
 */
void mu_stddemo_led_set(bool on);

/**
 * @brief Return true if the demo button is currently pressed.
 *
 * Note that the state of the button can change between the time the button
 * callback is triggered and the button state is read.
 */
bool mu_stddemo_button_is_pressed(void);

/**
 * @brief Put the processor into low power mode until an interrupt wakes it.
 */
void mu_stddemo_sleep(void);

/**
 * @brief Put the processor into low power mode until the Real Time Clock matches `at`
 * or until an interrupt occurs, whichever comes first.
 */
void mu_stddemo_sleep_until(mu_time_t at);


#ifdef __cplusplus
}
#endif

#endif // _MU_STDDEMO_H_
