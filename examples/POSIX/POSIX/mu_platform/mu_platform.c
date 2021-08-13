/**
 * MIT License
 *
 * Copyright (c) 2021 R. Dunbar Poor <rdpoor@gmail.com>
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

#include "mu_platform.h"
#include "mu_button_io.h"
#include "mu_led_io.h"
#include "mu_rtc.h"
#include "mu_ansi_term.h"
#include "mu_kbd_io.h"
#include "mu_signal.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <signal.h>

#include <unistd.h>
#include <sys/ioctl.h> // need this for getting the terminal size

// =============================================================================
// Private types and definitions

// =============================================================================
// Private (forward) declarations

static void read_ttysize(bool watch_for_changes);

#ifdef HAS_SIGNAL 
static void handle_sigwinch();
#endif

// =============================================================================
// Local storage

// =============================================================================
// Public code

void mu_platform_init(void) {
  mu_button_io_init();
  mu_led_io_init();
  mu_rtc_init();
  mu_kbd_io_init();
  mu_ansi_term_init();
  mu_signal_init();
  read_ttysize(true);
}

// =============================================================================
// Local (static) code

static void read_ttysize(bool watch_for_changes) {

  #ifdef TIOCGSIZE
      struct ttysize ts;
      ioctl(STDIN_FILENO, TIOCGSIZE, &ts);
      mu_ansi_term_set_ncols(ts.ts_cols);
      mu_ansi_term_set_nrows(ts.ts_lines);
  #elif defined(TIOCGWINSZ)
      struct winsize ts;
      ioctl(STDIN_FILENO, TIOCGWINSZ, &ts);
      mu_ansi_term_set_ncols(ts.ws_col);
      mu_ansi_term_set_nrows(ts.ws_row);
  #endif /* TIOCGSIZE */
  //printf("Terminal is %dx%d\n", mu_kbd_cols, mu_kbd_rows);

  #ifdef HAS_SIGNAL
    if(watch_for_changes)
      signal(SIGWINCH, handle_sigwinch);
  #endif
}

#ifdef HAS_SIGNAL 
static void handle_sigwinch() {
   read_ttysize(true);
}
#endif
