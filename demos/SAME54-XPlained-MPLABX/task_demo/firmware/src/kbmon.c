/**
 * MIT License
 *
 * Copyright (c) 2019 R. Dunbar Poor <rdpoor@gmail.com>
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
// includes

#include "kbmon.h"
#include "definitions.h"
#include "mu_sched.h"
#include <stddef.h>

// =============================================================================
// local types and definitions

// =============================================================================
// local (forward) declarations

// interrupt level callback when char is received.
static void kb_read_cb(uintptr_t context);

// =============================================================================
// local storage

static mu_sched_t *s_sched;
static mu_task_t *s_char_received_task;

// =============================================================================
// public code

void kbmon_init(mu_sched_t *sched, mu_task_t *char_received_task) {
  s_sched = sched;
  s_char_received_task = char_received_task;
  // install callback
  SERCOM2_USART_ReadCallbackRegister(kb_read_cb, (uintptr_t)NULL);
}

// =============================================================================
// local (static) code

static void kb_read_cb(uintptr_t context) {
  mu_sched_task_from_isr(s_sched, s_char_received_task);
}
