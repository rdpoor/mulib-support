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
#include "mulib.h"
// =============================================================================
// types and definitions


// Signature of the keyboard callback function
typedef void (*mu_kbd_io_callback_t)(unsigned char ch);

// =============================================================================
// declarations

/**
 * brief Initializes tty size as well as any signal handling
 */

void mu_kbd_io_init(void);

/**
 * @brief This callback will be called each time the user hits a key (assuming the tty is in non-canonical mode)
 */

void mu_kbd_io_set_callback(mu_kbd_io_callback_t cb);

/**
 * @brief Programmatically invoke the callback meant to handle user keystrokes.   This is used by examples/POSIX/POSIX/oblique/kbd_read.c 
 * in a separate posix thread that polls the keyboard.
 */



#ifdef __cplusplus
}
#endif

#endif /* #ifndef _MU_KBD_IO_H_ */
