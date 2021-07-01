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

#ifndef _MU_KBD_IO_H_
#define _MU_KBD_IO_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// includes
#include <termios.h>
#include "mulib.h"
// =============================================================================
// types and definitions


// Signature of the keyboard callback function
typedef void (*mu_kbd_io_callback_t)(unsigned char ch);

// =============================================================================
// declarations

void mu_kdb_io_init(void);

void mu_kbd_io_set_callback(mu_kbd_io_callback_t cb);

void fire_kbd_io_callback(char ch);

void mu_kbd_enter_noncanonical_mode(void);

void mu_kbd_exit_noncanonical_mode(void);

int mu_kbd_get_key_press(void);

int mu_kbd_ncols();

int mu_kbd_nrows();


#ifdef __cplusplus
}
#endif

#endif /* #ifndef _MU_KBD_IO_H_ */
