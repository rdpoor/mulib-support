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
#include "mu_ansi_term.h"

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
// =============================================================================
// Local types and definitions

#define STDIN_FILENO 0

// =============================================================================
// Local storage

static mu_kbd_io_callback_t s_kbd_io_cb;

struct termios saved_attributes;
static bool _has_saved_attributes = false;

static int mu_kbd_cols = 80;
static int mu_kbd_rows = 24;

// =============================================================================
// Local (forward) declarations

void mu_kbd_get_terminal_attributes(struct termios *terminal_attributes);
void mu_kbd_set_terminal_attributes(struct termios *terminal_attributes);

// =============================================================================
// Public code

void mu_bd_io_init(void) {
  mu_kbd_get_terminal_attributes(&saved_attributes); // so we can restore later
  _has_saved_attributes = true;
  mu_kbd_enter_noncanonical_mode();
  atexit(mu_kbd_exit_noncanonical_mode); // restores terminal attributes

// #ifdef TIOCGSIZE
//     struct ttysize ts;
//     ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
//     mu_ansi_cols = ts.ts_cols;
//     mu_ansi_rows = ts.ts_lines;
// #elif defined(TIOCGWINSZ)
//     struct winsize ts;
//     ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
//     mu_ansi_cols = ts.ws_col;
//     mu_ansi_rows = ts.ws_row;
// #endif /* TIOCGSIZE */
//     printf("Terminal is %dx%d\n", mu_ansi_cols, mu_ansi_rows);
//     signal(SIGWINCH, got_sigwinch);

}

void mu_kbd_io_set_callback(mu_kbd_io_callback_t cb) {
  s_kbd_io_cb = cb;
}

void fire_kbd_io_callback(char ch) {
  s_kbd_io_cb(ch);
}

// =============================================================================
// Local (static) code

// void got_sigwinch() {
//   printf("Got it\n");
// }

/**
 * @brief Assuming our stdin terminal is in non-canonical mode (set via mu_set_terminal_attributes)
 * this function checks stdin for the next user-entered character, but won't hang if there isn't one.
 * If wait_for_newlines was turned off then there will also be no waiting for newline
 * 
 */

int mu_kbd_get_key_press(void) {
    int ch;
    ch = getchar();
    if (ch < 0) {
        if (ferror(stdin)) { /* there was an error... */ }
        clearerr(stdin);
        /* there was no keypress */
        return 0;
    }
    if(ch == 3) exit(0); // ctrl-c special case
    return ch;
}

/**
 * @brief Change attributes on the terminal.
*/

void mu_kbd_set_terminal_attributes(struct termios *terminal_attributes) {
    tcsetattr(STDIN_FILENO, TCSANOW, terminal_attributes);
}
void mu_kbd_get_terminal_attributes(struct termios *terminal_attributes) {
      tcgetattr(STDIN_FILENO, terminal_attributes);      
}


int mu_kbd_ncols() {
  return mu_kbd_cols;
}
int mu_kbd_nrows() {
  return mu_kbd_rows;
}

void mu_kbd_enter_noncanonical_mode() {
  struct termios info;
  mu_kbd_get_terminal_attributes(&saved_attributes); // so we can restore later
  _has_saved_attributes = true;
 /* get current terminal attirbutes */
  bool canonical = false, echo_input = false, wait_for_newlines = false; // these could be passed in...
  tcgetattr(STDIN_FILENO, &info);          
  info.c_lflag &= ((ICANON && canonical) | (ECHO && echo_input)); // TODO make sure we're toggling these bits correctly
  info.c_cc[VMIN] = wait_for_newlines ? 1 : 0;
  info.c_cc[VTIME] = 0;         /* no timeout */
  tcsetattr(STDIN_FILENO, TCSANOW, &info);
  printf("ok");
}

void mu_kbd_exit_noncanonical_mode() {
  if(_has_saved_attributes)
    mu_kbd_set_terminal_attributes(&saved_attributes);
  printf( "%s%s\n", MU_ANSI_TERM_ESC, MU_ANSI_TERM_RESET); // undo any color settings
  mu_ansi_term_set_cursor_visible(true);
}
