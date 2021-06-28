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

#include "mu_led_io.h"
#include "mu_ansi_term.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

// =============================================================================
// Local types and definitions

// =============================================================================
// Local storage

bool s_led_state;

// =============================================================================
// Local (forward) declarations

// =============================================================================
// Public code

void mu_led_io_init(void) {
  mu_led_io_set(0, false);
  atexit(mu_ansi_term_exit_noncanonical_mode); // restores terminal attributes
}

// Draw a virtual LED at 0,0 on the screen.  Assumes an ANSI compiant terminal.
void mu_led_io_set(uint8_t led_id, bool on) {
  (void)led_id;

  s_led_state = on;   // track state
  //mu_ansi_term_save_cursor_position();

  // draw a green dot at [0,0]
  mu_ansi_term_set_foreground_color(on ? MU_ANSI_TERM_BRIGHT_GREEN : MU_ANSI_TERM_WHITE);
  mu_ansi_term_set_cursor_position(0, 0);
  puts("•\n");
  // restore color and cursor position
  mu_ansi_term_reset();
  //mu_ansi_term_restore_cursor_position();
 // mu_ansi_term_set_cursor_visible(true);
}

bool mu_led_io_get(uint8_t led_id) {
  (void)led_id;
  return s_led_state;
}

// =============================================================================
// Local (static) code
