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

#include "mu_kbd_io.h"
#include "driver_init.h"
#include <usart_basic.h>
#include <atomic.h>
#include <stddef.h>

// =============================================================================

// Local types and definitions

// =============================================================================
// Local storage

static mu_kbd_io_callback_t s_kbd_io_cb;

// =============================================================================
// Local (forward) declarations

static void handle_rx_isr(void);
// Normally we avoid extern declarations, but these two functions are not
// declared in usart_basic.h (though they should be).
extern void USART_0_default_rx_isr_cb(void);
extern void USART_0_default_udre_isr_cb(void);

// =============================================================================
// Public code

void mu_kbd_io_init(void) {
  // Set up to capture keyboard rx interrupts
  USART_0_set_ISR_cb(handle_rx_isr, RX_CB);
  USART_0_enable();
  ENABLE_INTERRUPTS();
  s_kbd_io_cb = NULL;
}

void mu_kbd_io_set_callback(mu_kbd_io_callback_t cb) {
  s_kbd_io_cb = cb;
}

// =============================================================================
// Local (static) code

static void handle_rx_isr(void) {
  // Arrive here at interrupt level when a character is received on the kbd.
  // TODO: verify that reading USARTE0.DATA leaves the data available
  uint8_t data = USARTE0.DATA;
  USART_0_default_rx_isr_cb();   // call the default handler...
  // If there is a user callback, call it...
  if (s_kbd_io_cb) {
    s_kbd_io_cb(data);
  }
}
