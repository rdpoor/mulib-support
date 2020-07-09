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

#ifndef _KBD_TASK_H_
#define _KBD_TASK_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include "mu_sched.h"
#include "mu_task.h"

// =============================================================================
// types and definitions

// Most tasks won't need to keep a reference to both the task and scheduler
// objects.  However, the kbd task is triggered from an interrupt, and the
// interrupt registration function accepts a single pointer.  From within the
// interrupt context, we need to access both the task and the scheduler.

typedef struct {
  mu_task_t *task;
  mu_sched_t *sched;
  uint8_t ch;              // one-character buffer for serial read
} kbd_ctx_t;

// =============================================================================
// declarations

mu_task_t *kbd_task_init(mu_task_t *kbd_task,
                         kbd_ctx_t *kbd_ctx,
                         mu_sched_t *sched);

/**
 * @brief Configure the usart for low power mode or regular mode.
 *
 * Disable the USART when the system is idle
 */
void kbd_task_set_low_power_mode(bool low_power);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef _KBD_TASK_H_ */
