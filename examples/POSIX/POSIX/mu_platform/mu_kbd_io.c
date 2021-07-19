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
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <termios.h>
#include <pthread.h>

// =============================================================================
// Local types and definitions

#define STDIN_FILENO 0

// =============================================================================
// Local storage

static mu_kbd_io_callback_t s_kbd_io_cb;
static struct termios saved_attributes;
static bool _has_saved_attributes = false;
static bool _tty_is_in_non_canonical_mode = false;
pthread_t thread_id;


// =============================================================================
// Local (forward) declarations

static void mu_kbd_get_terminal_attributes(struct termios *terminal_attributes);
static void mu_kbd_set_terminal_attributes(struct termios *terminal_attributes);

static void fire_kbd_io_callback(char ch);

static void mu_kbd_enter_noncanonical_mode(void);


static void mu_kbd_exit_noncanonical_mode(void);

static int mu_kbd_get_key_press(void);

static void start_kbd_reader_thread(void);

static void *reader_thread(void* vargp);



// =============================================================================
// Public code

void mu_kbd_io_init(void) {
  mu_kbd_enter_noncanonical_mode();
  atexit(mu_kbd_exit_noncanonical_mode); // restores terminal attributes
  start_kbd_reader_thread();
}

void mu_kbd_io_set_callback(mu_kbd_io_callback_t cb) {
  s_kbd_io_cb = cb;
}


// =============================================================================
// Local code


static void fire_kbd_io_callback(char ch) {
  s_kbd_io_cb(ch);
}

static int mu_kbd_get_key_press(void) {
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

static void mu_kbd_enter_noncanonical_mode() {
  static struct termios info;
  bool canonical = false, echo_input = false, wait_for_newlines = false; 
  if(_tty_is_in_non_canonical_mode) return;
  mu_kbd_get_terminal_attributes(&saved_attributes); // so we can restore later
  _has_saved_attributes = true;
  _tty_is_in_non_canonical_mode = true;
  tcgetattr(STDIN_FILENO, &info);          
  info.c_lflag &= ((ICANON && canonical) | (ECHO && echo_input)); 
  info.c_cc[VMIN] = wait_for_newlines ? 1 : 0;
  info.c_cc[VTIME] = 0;         /* no timeout */
  mu_kbd_set_terminal_attributes(&info);
 }

static void mu_kbd_exit_noncanonical_mode() {
  if(!_tty_is_in_non_canonical_mode) return;
  _tty_is_in_non_canonical_mode = false;
  if(_has_saved_attributes) {
    mu_kbd_set_terminal_attributes(&saved_attributes);
  } else {
    //printf("mu_kbd_exit_noncanonical_mode has no saved attributed so resetting hard...\n");
    static struct termios info;
    bool canonical = true, echo_input = true;
    bool wait_for_newlines = true; 
    tcgetattr(STDIN_FILENO, &info);          
    info.c_lflag |= ((ICANON && canonical) | (ECHO && echo_input));
    info.c_cc[VMIN] = wait_for_newlines ? 1 : 0;
    info.c_cc[VTIME] = 0;         /* no timeout */
    mu_kbd_set_terminal_attributes(&info);
  }
  printf("\33[0m"); // ansi reset ?25h
  printf("\33[?25h"); // ansi show cursor
}

static void mu_kbd_set_terminal_attributes(struct termios *terminal_attributes) {
  tcsetattr(STDIN_FILENO, TCSANOW, terminal_attributes);
}

static void mu_kbd_get_terminal_attributes(struct termios *terminal_attributes) {
  printf("mu_kbd_get_terminal_attributes\n");
  tcgetattr(STDIN_FILENO, terminal_attributes);      
}



// this allows POSIX to mimic MCU behavior which typically uses an isr to send
// info about each keypress via the callback at s_kbd_io_cb()
static void start_kbd_reader_thread(void) {
  pthread_create(&thread_id, NULL, reader_thread, NULL);
}

static void *reader_thread(void* vargp)
{
    while(1) {
      char ch = mu_kbd_get_key_press();
      if(ch) {
        fire_kbd_io_callback(ch);
      }
    }
    return NULL;
}



//static void handle_rx_isr(void) {
  // Arrive here at interrupt level when a character is received on the kbd.
  // TODO: verify that reading USARTE0.DATA leaves the data available

  //uint8_t data = USARTE0.DATA;
  //USART_0_default_rx_isr_cb();   // call the default handler...
  // If there is a user callback, call it...
 // if (s_kbd_io_cb) {
 //   s_kbd_io_cb(data);
//  }
//}

