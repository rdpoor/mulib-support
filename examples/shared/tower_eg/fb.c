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
#include <stdint.h>
#include <string.h>

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

void fb_clear(void) {
  memset(s_fb.backing_store, 0, s_fb.width * s_fb.height);
}

void fb_draw(uint8_t x, uint8_t y, char ch) {
  s_fb.backing_store[x + y * s_fb.width] = ch;
}

void fb_flush(void) {
  for (uint8_t y = 0; y < s_fb.height; y++) {
    for (uint8_t x=0; x< s_fb.width; x++) {
      int idx = x + y * s_fb.width;
      // This version blindly prints the entire buffer to the screen.  Lots of
      // room for improvement.
      char ch = s_fb.backing_store[idx];
      putc(ch);
      s_fb.display_store[idx] = ch;
    }
    putc('\n');
  }
}


// =============================================================================
// Local (static) code
