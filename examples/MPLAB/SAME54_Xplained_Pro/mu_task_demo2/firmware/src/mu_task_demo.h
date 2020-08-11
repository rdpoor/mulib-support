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

#ifndef _MU_TASK_DEMO_H_
#define _MU_TASK_DEMO_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include <stdbool.h>
#include <mulib.h>

// =============================================================================
// types and definitions

#define MU_TASK_DEMO_VERSION "1.0.0"

// =============================================================================
// declarations

void mu_task_demo_init(void);

void mu_task_demo_step(void);

void mu_task_demo_start_led_task(void);

void mu_task_demo_stop_led_task(void);

void mu_task_demo_start_screen_update_task(void);

void mu_task_demo_stop_screen_update_task(void);

void mu_task_demo_set_low_power_mode(bool low_power);

bool mu_task_demo_is_low_power_mode(void);

mu_substr_t *mu_task_demo_get_screen_buffer(void);

mu_sched_t *mu_task_demo_get_scheduler(void);

mu_task_t *mu_task_demo_get_screen_redraw_task(void);

size_t mu_task_demo_get_task_count(void);

mu_task_t *mu_task_demo_get_tasks(void);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _MU_TASK_DEMO_H_ */
