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

#include "disk.h"
#include <stdint.h>
#include <stdbool.h>

// =============================================================================
// Local types and definitions

// =============================================================================
// Local storage

// =============================================================================
// Local (forward) declarations

// =============================================================================
// Public code

void disk_init(disk_t *disk, uint8_t width) {
  disk->next = NULL;
  disk->width = width;
}

uint8_t disk_get_width(disk_t *disk) {
  return disk->width;
}

uint8_t disk_get_xpos(disk_t *disk) {
  return disk->xpos;
}

uint8_t disk_get_ypos(disk_t *disk) {
  return disk->ypos;
}

void disk_set_position(disk_t *disk, uint8_t xpos, uint8_t ypos) {
  disk->xpos = xpos;
  disk->ypos = ypos;
}

void disk_move_x(disk_t *disk, int dx) {
  disk->xpos += dx;
}

void disk_move_y(disk_t *disk, int dy) {
  disk->ypos += dy;
}

void disk_draw(disk_t *disk) {
  int radius = disk->width/2;  // truncate towards 0
  for (int x = -radius; x < radius; x++) {
    fb_draw(disk->xorg+x, 0, '=');
  }
}

// =============================================================================
// Local (static) code
