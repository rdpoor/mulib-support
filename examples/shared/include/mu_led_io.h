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
 * @file mu_led_io.h
 *
 * @brief Control a platform-specific LED.
 *
 * mu_led_io defines a platform-specific LED.  This is commonly required by
 * example applications, but could be used in production code if needed.
 */

#ifndef _MU_LED_IO_H_
#define _MU_LED_IO_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include <stdbool.h>
#include <stdint.h>

// =============================================================================
// types and definitions

// The canonical LED used in example code.
#define MU_LED_0   0

// =============================================================================
// declarations

/**
 * @brief Initialize the LED.
 */
void mu_led_io_init(void);

/**
 * @brief Turn on or off an LED
 */
void mu_led_io_set(uint8_t led_id, bool on);

/**
 * @brief Get the state of an LED.
 */
bool mu_led_io_get(uint8_t led_id);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef __TEMPLATE_H_ */
