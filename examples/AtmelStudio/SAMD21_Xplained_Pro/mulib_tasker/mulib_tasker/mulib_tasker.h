/**
 * MIT License
 *
 * Copyright (c) 2020 R. Dunbar Poor <rdpoor@gmail.com>
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

#ifndef _MULIB_TASKER_H_
#define _MULIB_TASKER_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include <stdbool.h>

// =============================================================================
// types and definitions

#define MULIB_TASKER_VERSION "1.0.0"

// =============================================================================
// declarations

void mulib_tasker_init(void);

void mulib_tasker_step(void);

void mulib_tasker_start_led_task(void);

void mulib_tasker_stop_led_task(void);

void mulib_tasker_start_screen_update_task(void);

void mulib_tasker_stop_screen_update_task(void);

void mulib_tasker_set_low_power_mode(bool low_power);

bool mulib_tasker_is_low_power_mode(void);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _MULIB_TASKER_H_ */
