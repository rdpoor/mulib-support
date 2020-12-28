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

#ifndef _BLINK_BASIC_H_
#define _BLINK_BASIC_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include "mulib.h"
#include <stdint.h>

// =============================================================================
// types and definitions

// =============================================================================
// declarations

/**
 * @brief Initialize a blink_basic task to turn an LED on and off periodically.
 *
 * @param on_time_ms How long the LED is on, in milliseconds.
 * @param off_time_ms How long the LED is off, in milliseconds.
 * @return An initialized mu_task, ready to be scheduled.
 */
mu_task_t *blink_basic_init(uint16_t on_time_ms, uint16_t off_time_ms);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _BLINK_BASIC_H_ */
