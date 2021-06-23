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

#ifndef _MU_STDDEMO_H_
#define _MU_STDDEMO_H_

#ifdef __cplusplus
extern "C" {
#endif

// =============================================================================
// Includes

#include <stdbool.h>
#include <stdio.h>
#include "mu_time.h"

#define ANSI_ESC "\33["
#define ANSI_RESET "0m"
#define ANSI_RED "31m"
#define ANSI_GREEN "32m"
#define ANSI_YELLOW "33m"
#define ANSI_BLUE "34m"
#define ANSI_MAGENTA "35m"
#define ANSI_CYAN "36m"
#define ANSI_INVERSE "7m"



// =============================================================================
// Types and definitions


// =============================================================================
// Functon declarations (public)
  /**
 * @brief invoke the ANSI terminal 'bell' code
 *
 */

void mu_stddemo_terminal_bell();

/**
 * @brief Initialize the mu_stddemo_support system.
 *
 * @param canonical boolean flag for terminal canonical mode.   setting to false means getchar() works before linefeeds
 * @param echo_input boolean flag, when false the keystrokes are not displayed on the terminal.  
 * @param wait_for_newlines boolean flag
 * 
 * Note that it's good practice to preserve the state of the terminal attributes prior to calling this, so that they can be restored before exitting the program
 * 
 */

void mu_set_terminal_attributes(bool canonical, bool echo_input, bool wait_for_newlines);

typedef void (*mu_stddemo_set_led_cb)(bool led_state);

void set_led_callback(mu_stddemo_set_led_cb led_cb);

/**
 * @brief Initialize the mu_stddemo_support system.
 *
 * @param button_cb Function to call from interrupt level when the user button
 * is pressed.  Set to NULL to inhibit callbacks.
 */
typedef void (*mu_stddemo_button_cb)(bool button_state);

void mu_stddemo_init(mu_stddemo_button_cb button_cb);

void set_terminal_foreground_color(char color_id_256);
void terminal_clear();
void terminal_erase_last_line();
void terminal_reset();
/**
 * @brief Set the demo LED on or off.
 */
void mu_stddemo_led_set(bool on);

/**
 * @brief Return true if the demo button is currently pressed.
 *
 * Note that the state of the button can change between the time the button
 * callback is triggered and the button state is read.
 */
bool mu_stddemo_button_is_pressed(void);

/**
 * @brief Called from ISR when button is pressed.
 */
void mu_stddemo_on_button_press(void);

int mu_get_key_press(void);

#ifdef __cplusplus
}
#endif

#endif // _MU_STDDEMO_H_
