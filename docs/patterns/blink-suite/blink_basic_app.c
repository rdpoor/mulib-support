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

 // =============================================================================
 // Includes

 #include "blink_basic_app.h"

 #include "mulib.h"
 #include "blink_bsp.h"
 #include "blink_basic.h"

 // =============================================================================
 // Local types and definitions

 #define ON_TIME_MS 10
 #define OFF_TIME_MS (500 - ON_TIME_MS)

 #define ISR_QUEUE_SIZE 8

 // =============================================================================
 // Local storage

 // Allocate storage for the scheduler's Interrupt Service Routine queue.
 // TODO: use the task's link field for ISR queue rather than this...
 static mu_spscq_item_t s_isr_queue_items[ISR_QUEUE_SIZE];

 // =============================================================================
 // Local (forward) declarations

 // =============================================================================
 // Public code

 void blink_basic_app_init(void) {
   blink_bsp_init();
   // TODO: refactor mu_time_init(); mu_sched_init() => mu_init()
   mu_time_init();
   mu_sched_init(s_isr_queue_items, ISR_QUEUE_SIZE);

   mu_task_t *blink_task = blink_basic_init(ON_TIME_MS, OFF_TIME_MS);
   mu_sched_task_now(blink_task);
 }

 void blink_basic_app_step(void) {
   mu_sched_step();
 }

 // =============================================================================
 // Local (static) code
