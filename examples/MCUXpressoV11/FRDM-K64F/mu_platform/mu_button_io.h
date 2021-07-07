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

/**
 * @file mu_button_io.h
 *
 * @brief Interact with a platform-specific button.
 *
 * mu_button_io defines a platform-specific button.  This is commonly required
 * example applications, but could be used in production code if needed.
 *
 * In the case of the POSIX platform, there is no physical button.  Instead, we
 * use terminal interactions and watch for a special key sequence (TBD).  Upon
 * detecting the sequence, mu_button_io invokes the user-supplied callback
 * function.
 */

#ifndef _MU_BUTTON_IO_H_
#define _MU_BUTTON_IO_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include <stdbool.h>
#include <stdint.h>

// =============================================================================
// types and definitions

// The canonical button used in example code.
#define MU_BUTTON_0   0

// Signature of the button callback function
typedef void (*mu_button_io_callback_t)(uint8_t button_id, bool pressed);

// =============================================================================
// declarations

/**
 * @brief Initialize the button support.
 */
void mu_button_io_init(void);

/**
 * @brief Set the user-supplied function to be called upon button presses.
 *
 * @param cb User-supplied callback function, called when the button changes
 * state.  A value of NULL will disable callbacks.
 */
void mu_button_io_set_callback(mu_button_io_callback_t cb);

/**
 * @brief Get the state of the named button.
 */
bool mu_button_io_get_button(uint8_t button_id);

// =============================================================================
// This is not public functions, but need to be declared for the ISR in
// driver_isr.c

// TODO: Design pattern question: perhaps this is the one place where we don't
// provide a declaration in the .h file and instead use an extern declaration
// in the driver_isr.c file instead.

// void mu_button_io_on_button_change(void);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef __TEMPLATE_H_ */
