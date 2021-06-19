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

/**
 * @file disk.h
 *
 * Represents a disk in the Tower of Hanoi
 */

#ifndef DISK_H_
#define DISK_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

#include <stdint.h>

// =============================================================================
// types and definitions

typedef struct _disk {
  struct _disk *next;
  uint8_t width;
  uint8_t xpos;
  uint8_t ypos;
} disk_t;

// =============================================================================
// declarations

void disk_init(disk_t *disk, uint8_t width);

void disk_next(disk_t *disk);

uint8_t disk_get_width(disk_t *disk);

uint8_t disk_get_xpos(disk_t *disk);

uint8_t disk_get_ypos(disk_t *disk);

void disk_set_position(disk_t *disk, uint8_t xpos, uint8_t ypos);

void disk_move_x(disk_t *disk, int dx);

void disk_move_y(disk_t *disk, int dy);

void disk_draw(disk_t *disk);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef DISK_H_ */
