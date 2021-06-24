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
 * @file tower.h
 *
 * Tower Of Hanoi example, using ASCII art and ANSI Escape Sequences.
 */

#ifndef TOWER_H_
#define TOWER_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes

// =============================================================================
// types and definitions

#define N_DISKS 5
#define POLE_WIDTH (2 * N_DISKS + 1)
#define POLE_HEIGHT (N_DISKS)
#define POLE_YPOS 0
#define CRUISING_ALTITUDE (POLE_HEIGHT + 3)

// =============================================================================
// declarations

void tower_init(void);

void tower_step(void);

void tower_draw(void);

#ifdef __cplusplus
}
#endif

#endif /* #ifndef TOWER_H_ */
