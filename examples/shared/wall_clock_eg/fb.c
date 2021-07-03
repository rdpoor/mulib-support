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

#include "fb.h"

#include <mulib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

// =============================================================================
// Local types and definitions

typedef struct {
  char *backing_store;
  char *display_store;
  uint8_t width;
  uint8_t height;
} fb_t;

// =============================================================================
// Local storage

static fb_t s_fb;

// =============================================================================
// Local (forward) declarations

// =============================================================================
// Public code

void fb_init(uint8_t width,
             uint8_t height,
             char *backing_store,
             char *display_store) {
  s_fb.width = width;
  s_fb.height = height;
  s_fb.backing_store = backing_store;
  s_fb.display_store = display_store;
}

void fb_erase(void) {
  mu_ansi_term_home();
  mu_ansi_term_clear_screen();
  memset(s_fb.backing_store, ' ', s_fb.width * s_fb.height);
  memset(s_fb.display_store, ' ', s_fb.width * s_fb.height);
}

void fb_clear(void) {
  memset(s_fb.backing_store, ' ', s_fb.width * s_fb.height);
}

char *fb_row_ref(uint8_t row_number) {
  return &s_fb.backing_store[s_fb.width * row_number];
}

void fb_draw(uint8_t x, uint8_t y, char ch) {
  // [x=0, y=0] is at bottom left
  s_fb.backing_store[x + (s_fb.height - y - 1) * s_fb.width] = ch;
}

void fb_flush(void) {
  int idx = 0;
  for (uint8_t y = 0; y < s_fb.height; y++) {
    for (uint8_t x=0; x< s_fb.width; x++) {
      char ch = s_fb.backing_store[idx];
      if (ch != s_fb.display_store[idx]) {
        mu_ansi_term_set_cursor_position(y, x);
        putchar(ch);
        s_fb.display_store[idx] = ch;
      }
      idx++;
    }
  }
  mu_ansi_term_home();
}

// =============================================================================
// Local (static) code
