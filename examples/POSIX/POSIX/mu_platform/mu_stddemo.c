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

#include "mu_stddemo.h"
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

// =============================================================================
// Private types and definitions

// =============================================================================
// Private (forward) declarations

void mu_stddemo_cleanup(void);

// =============================================================================
// Local storage

static struct termios saved_attributes;

static mu_stddemo_button_cb s_button_cb;

static mu_stddemo_set_led_cb _set_led_cb = false;

// =============================================================================
// Public code

// we can connect this button press callback (button_cb) to keypresses in lieu of physical buttons here in POSIX

void mu_button_io_set_callback(mu_stddemo_button_cb button_cb) {
    s_button_cb = button_cb;
    /* Save the terminal attributes so we can restore them later, in case we change them. */
    tcgetattr (STDIN_FILENO, &saved_attributes);
    atexit(mu_stddemo_cleanup); // restores terminal attributes
}

/**
 * @brief Restore the terminal to its original state
 */
void mu_stddemo_cleanup(void) {
    tcsetattr (STDIN_FILENO, TCSANOW, &saved_attributes); // restore terminal attributes
    printf( "%s%s", ANSI_ESC, ANSI_RESET); // undo any color settings
}

/**
 * @brief use this to simulate the user pressing a button
*/

void mu_stddemo_on_button_press(void) {
  if (s_button_cb != NULL) {
      s_button_cb(true);
  }
}

/**
 * @brief register to receive a callback whenever the state of the LED (virtual because we're in POSIX) changes
 *
*/
void set_led_callback(mu_stddemo_set_led_cb led_cb) {
    _set_led_cb = led_cb;
}

/**
 * @brief Set the LED (virtual because we're in POSIX)  on or off.  Here in posix we allow for a callback (that can customize other visual indicators) 
 * and fallback to a simple terminal bell/flash 
 */
void mu_led_io_set(bool on) {
    if(_set_led_cb)
        _set_led_cb(on);
    else mu_stddemo_terminal_bell();
}

/**
 * @brief Return true if the demo button is currently pressed.
 *
 * Note that the state of the button can change between the time the button
 * callback is triggered and the button state is read.
 */
bool mu_stddemo_button_is_pressed(void) {
    return false;
}

// =============================================================================
// Terminal Routines

void mu_stddemo_terminal_bell() {
    printf("\a"); // ansi terminal bell / flash
}

void set_terminal_foreground_color(char color_id_256) {
    printf("%s38;5;%dm",ANSI_ESC,color_id_256);
}

void terminal_clear() {
    printf("%s1A%s2J",ANSI_ESC,ANSI_ESC);
}

void terminal_erase_last_line() {
    printf("%s1A%sK",ANSI_ESC,ANSI_ESC);
}

void terminal_reset() {
    printf( "%s%s", ANSI_ESC, ANSI_RESET); // undo any color settings
}

/**
 * @brief Assuming our stdin terminal is in non-canonical mode (set via mu_set_terminal_attributes)
 * this function checks stdin for the next user-entered character, but won't hang if there isn't one.
 * If wait_for_newlines was turned off then there will also be no waiting for newline
 * 
 */

int mu_get_key_press(void) {
    int ch;
    ch = getchar();
    if (ch < 0) {
        if (ferror(stdin)) { /* there was an error... */ }
        clearerr(stdin);
        /* there was no keypress */
        return 0;
    }
    return ch;
}

/**
 * @brief Change attributes on the terminal.
*/

void mu_set_terminal_attributes(bool canonical, bool echo_input, bool wait_for_newlines) {
    struct termios info;
    /* Make sure stdin is a terminal. */
    if (!isatty (STDIN_FILENO)) {
      fprintf (stderr, "Not a terminal.\n");
     return;
    }

    /* get current terminal attirbutes */
    tcgetattr(STDIN_FILENO, &info);          
    info.c_lflag &= ((ICANON && canonical) | (ECHO && echo_input)); // TODO make sure we're toggling these bits correctly
    info.c_cc[VMIN] = wait_for_newlines ? 1 : 0;
    info.c_cc[VTIME] = 0;         /* no timeout */
    tcsetattr(STDIN_FILENO, TCSANOW, &info);
}



