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

#include "mu_kbd_io.h"
#include "extras/mu_ansi_term.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
// =============================================================================
// Local types and definitions

#define STDIN_FILENO 0
typedef struct {
  int stud;
} termios;

// =============================================================================
// Local storage

static bool _has_saved_attributes = false;
static bool _tty_is_in_non_canonical_mode = false;
static int mu_kbd_cols = 80;
static int mu_kbd_rows = 24;

// =============================================================================
// Local (forward) declarations

static void read_ttysize();
static void handle_sigwinch();

// =============================================================================
// Public code

void mu_kbd_io_init(void) {
}

void mu_kbd_io_set_callback(mu_kbd_io_callback_t cb) {
  //s_kbd_io_cb = cb;
}

void fire_kbd_io_callback(char ch) {
  //s_kbd_io_cb(ch);
}

int mu_kbd_ncols() {
  return mu_kbd_cols;
}

int mu_kbd_nrows() {
  return mu_kbd_rows;
}

int mu_kbd_get_key_press(void) {
    int ch;
    ch = getchar();
    if (ch < 0) {
        if (ferror(stdin)) { /* there was an error... */ }
        clearerr(stdin); // call clearerr regardless
        /* there was no keypress */
        return 0;
    }
    if(ch == 3) exit(0); // Ctrl-C
    return ch;
}

void mu_kbd_enter_noncanonical_mode() {
  
 }

void mu_kbd_exit_noncanonical_mode() {
  
}

// =============================================================================
// Local (static) code

static void handle_sigwinch() {
   //read_ttysize();
}

// static void mu_kbd_set_terminal_attributes(struct termios *terminal_attributes) {
// }

// static void mu_kbd_get_terminal_attributes(struct termios *terminal_attributes) {
// }

static void read_ttysize() {
}


