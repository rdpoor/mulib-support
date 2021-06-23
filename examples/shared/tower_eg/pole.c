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

#include "pole.h"

#include "fb.h"
#include "tower.h"
#include <stdint.h>

// =============================================================================
// Local types and definitions

// =============================================================================
// Local storage

// =============================================================================
// Local (forward) declarations

// =============================================================================
// Public code

/**
 * Here is how a pole with w=9, h=5 is drawn in ASCII art:
 *
 *      #
 *      #
 *      #
 *      #
 *      #
 *  ####o####   // NB: x=0,y=0 is at 'o'.  y increases upwards.
 */

void pole_init(pole_t *pole, uint8_t xpos) {
  pole->xpos = xpos;
  pole->disks = NULL;
}

void pole_draw(pole_t *pole) {
  // draw peg
  for (int i=0; i<POLE_HEIGHT; i++) {
    fb_draw(pole->xpos, POLE_YPOS + i + 1, '#');
  }
  // draw base
  for (int i=0; i<POLE_WIDTH; i++) {
    fb_draw(pole->xpos+i-(POLE_WIDTH/2), 0, '#');
  }
}

uint8_t pole_top_x(pole_t *pole) {
  return pole->xpos;
}

uint8_t pole_top_y(pole_t *pole) {
  return POLE_YPOS + pole_disk_count(pole);
}


uint8_t pole_disk_count(pole_t *pole) {
  uint8_t count = 0;
  disk_t *d = pole->disks;

  while (d) {
    count += 1;
    d = d->next;
  }
  return count;
}

void pole_push(pole_t *pole, disk_t *disk) {
  disk->next = pole->disks;
  pole->disks = disk;
}

disk_t *pole_pop(pole_t *pole) {
  disk_t *d = pole->disks;
  if (d != NULL) {
    pole->disks = d->next;
    d->next = NULL;
  }
  return d;
}

disk_t *pole_top(pole_t *pole) {
  return pole->disks;
}

// =============================================================================
// Local (static) code
