/**
 * MIT License
 *
 * Copyright (c) 2021 Klatu Networks, Inc
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
 * @brief Example demonstrating the use of the mu_scheduler to drive terminal animations.
 * Content from Oblique Strategies created by Brian Eno and Peter Schmidt
 */

// =============================================================================
// Includes

#include "hexagrams.h"
#include "i_ching.h"

#include <stdio.h>
#include <stdlib.h>
#include <mulib.h>

// =============================================================================
// Local types and definitions


#define VERSION "1.0"

// =============================================================================
// Local (forward) declarations

static void button_cb(uint8_t button_id, bool button_is_pressed);
static void kbd_cb(unsigned char ch);
static mu_duration_t wait_for_user();
static char get_user_coin_toss();
static void display_reading_for_lines(char *user_lines);

// =============================================================================
// Local storage

static bool user_hit_something = false;
static char _most_recent_character;
static char user_lines[7] = "999999";

// =============================================================================
// Public code

/**
 * @brief Walk the user through the process of contemplating their question and casting "coins" in the traditional way
 * 
 * 
 */

void i_ching_init() {
  mulib_init();
  mu_button_io_set_callback(button_cb);
  mu_kbd_io_set_callback(kbd_cb);
  mu_ansi_term_clear_screen();
  mu_ansi_term_home();

  // print_hexagram_info(hexagram_number_from_user_lines("778888"));
  //exit(0);

  printf("Contemplate your question.\nPress user button or any key to begin casting...\n\n");

  mu_ansi_term_set_cursor_visible(false);
  mu_ansi_term_clear_screen();

  for(int i = 0; i < 6; i++) {
    mu_ansi_term_home();
    user_lines[5 - i] = get_user_coin_toss(); // '6','7','8' or '9' -- note that we store the foundation in the rightmost character (so right to left) in order to align with our binary sk notation 0b00111111
    printf("\n");
    putchar(user_lines[i]);
  }
 
  display_reading_for_lines(user_lines);
}

void i_ching_step() {
  mu_sched_step();
  exit(0); // nothing to do here -- the party happened in i_ching_init
}


// =============================================================================
// Local (static) code

static void button_cb(uint8_t button_id, bool button_is_pressed) {
  (void)button_id;
  user_hit_something = true;
}

static void kbd_cb(unsigned char ch) {
  _most_recent_character = ch; 
  user_hit_something = true;
}

static mu_duration_t wait_for_user() {
  user_hit_something = false;
  mu_time_t start_time = mu_rtc_now();
  while (!user_hit_something) {
  }
  user_hit_something = false;
  return mu_time_difference(mu_rtc_now(), start_time);
}

// the time between keypresses determines the value of the coin flip.
// no need for the additional obfuscation of seeding an rng, 
// here it is entirely the user's actions which directly determines (in an uncontrollable and unpredictable way) the result of the coin flip

static char get_user_coin_toss() {
  uint8_t answer = 0, n_tosses = 3;
  bool coin_value;
  mu_duration_t wait_duration;
  while(n_tosses--) {
    mu_ansi_term_home();
    printf("\nhit %d keys to toss...",n_tosses);
    wait_duration = wait_for_user();
    coin_value = (wait_duration >> 4) % 2; // reduce the clock resolution a bit to shift the stochasticism to a more human scale
    answer += coin_value ? 3 : 2;
  }
  if(answer == 6) return '6';
  if(answer == 7) return '7';
  if(answer == 8) return '8';
  if(answer == 9) return '9';
  return 'x';
}

static void display_reading_for_lines(char *user_lines) {
  mu_ansi_term_clear_screen();
  mu_ansi_term_home();
  printf("coin tosses: %s\n\n",user_lines);
  draw_user_lines(user_lines);
  int num = hexagram_number_from_user_lines(user_lines);
  print_hexagram_info(num);
  char *changed_lines = change_user_lines(user_lines);
  int secondary_num = hexagram_number_from_user_lines(changed_lines);
  if(num != secondary_num) {
    printf("CHANGES:\n\n");
    print_analaysis_of_changing_lines(user_lines);
    printf("Secondary hexagram:\n\n");
    draw_user_lines(changed_lines);
    print_hexagram_info(secondary_num);
  }
}
