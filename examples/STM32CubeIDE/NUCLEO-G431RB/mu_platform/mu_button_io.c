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

#include "mu_button_io.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// =============================================================================
// Local types and definitions

// =============================================================================
// Local storage

static mu_button_io_callback_t s_button_io_cb;

// =============================================================================
// Local (forward) declarations

// =============================================================================
// Public code

void mu_button_io_init(void) {
  s_button_io_cb = NULL;
}

void mu_button_io_set_callback(mu_button_io_callback_t cb) {
  s_button_io_cb = cb;
}

void mu_button_io_on_button_press(unsigned char button_id) {
  if (s_button_io_cb != NULL) {
    s_button_io_cb(button_id, true);
  }
}

// void mu_button_io_on_button_change(void) {
//   if (s_button_io_cb) {
//     s_button_io_cb(MU_BUTTON_0, USER_BUTTON_get_level());
//   }
// }

// =============================================================================
// Local (static) code
