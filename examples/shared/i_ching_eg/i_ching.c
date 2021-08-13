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


#define VERSION "1.01"

// =============================================================================
// Local (forward) declarations

static void button_cb(uint8_t button_id, bool button_is_pressed);
static void kbd_cb(unsigned char ch);
static mu_duration_t wait_for_user(bool show_message);
static char get_user_coin_toss();
static void print_reversed(char *wut);
static void print_emphasized(char *wut);
static void present_reading_for_lines(char *user_lines);
static void print_hexagram_info(int hexagram_number);

// =============================================================================
// Local storage

static bool user_hit_something = false;
static char _most_recent_character;
static char user_lines[7];

// =============================================================================
// Public code

/**
 * @brief Walk the user through the process of contemplating their question and casting "coins" in the traditional way
 */
void i_ching_init() {
  mulib_init();
  mu_button_io_set_callback(button_cb);
  mu_kbd_io_set_callback(kbd_cb);
  mu_ansi_term_clear_screen();
  mu_ansi_term_home();
  mu_ansi_term_set_cursor_visible(false);

  printf("\nContemplate your question.\n\n");
  print_reversed("Press user button or any key to begin casting...\n\n");
  wait_for_user(false);
  mu_ansi_term_clear_screen();

  printf("hit keys to throw coins...");

  memset(user_lines,0,7);
  for(int i = 0; i < 6; i++) {
    mu_ansi_term_home();
    user_lines[5 - i] = get_user_coin_toss(); 
    // '6','7','8' or '9' -- note that we store the foundation in the rightmost character (so right to left) in order to align with our binary sk notation 0b00111111
    printf("\n%s",&user_lines[5 - i]);
  }
  present_reading_for_lines(user_lines);
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

static mu_duration_t wait_for_user(bool show_message) {
  user_hit_something = false;
  mu_time_t start_time = mu_rtc_now();
  if(show_message) {
    printf("\n");
    print_reversed("Any key to continue...");
  }
  while (!user_hit_something) {
  }
  if(show_message) {
    mu_ansi_term_clear_line();
  }
  user_hit_something = false;
  if(_most_recent_character == 'q')
    exit(0);
  return mu_time_difference(mu_rtc_now(), start_time);
}

static void print_reversed(char *wut) {
    mu_ansi_term_set_colors(MU_ANSI_TERM_BRIGHT_YELLOW, MU_ANSI_TERM_GRAY);
    printf("%s",wut);
    mu_ansi_term_set_colors(MU_ANSI_TERM_DEFAULT_COLOR, MU_ANSI_TERM_DEFAULT_COLOR);
}

static void print_emphasized(char *wut) {
    mu_ansi_term_set_colors(MU_ANSI_TERM_YELLOW, MU_ANSI_TERM_DEFAULT_COLOR);
    printf("%s",wut);
    mu_ansi_term_set_colors(MU_ANSI_TERM_DEFAULT_COLOR, MU_ANSI_TERM_DEFAULT_COLOR);
}

// the time between keypresses determines the value of the coin flip.
// no need for the additional obfuscation of seeding an rng, 
// here it is entirely the user's actions which directly determines (in an uncontrollable and unpredictable way) the result of the coin flip

static char get_user_coin_toss() {
  uint8_t answer = 0, n_tosses = 3;
  bool coin_value;
  mu_duration_t wait_duration;
  while(n_tosses--) {
    wait_duration = wait_for_user(false);
    if(n_tosses == 2) {
      printf("      ");
      mu_ansi_term_home();
    }
    coin_value = (wait_duration >> 4) % 2; // reduce the clock resolution a bit to shift the stochasticism to a more human scale
    putchar(coin_value ? 'X' : 'O');
    putchar(' ');
    answer += coin_value ? 3 : 2;
  }
  printf("\n");
  return 48 + answer; // turn 6 into '6'
}

static void present_reading_for_lines(char *user_lines) {
  char *cast_user_lines[2];
  cast_user_lines[0] = user_lines;
  char *changed_lines = change_user_lines(user_lines);
  cast_user_lines[1] = changed_lines;
  int num = hexagram_number_from_user_lines(user_lines);
  int secondary_num = hexagram_number_from_user_lines(changed_lines);
  const i_ching_hexagram *hex = get_hexagram(num); 
  bool has_changes = secondary_num != num;

  mu_ansi_term_clear_screen();
  mu_ansi_term_home();
  printf("\n");

  // show the relevant hexagrams
  draw_multiple_user_lines(cast_user_lines,  has_changes ? 2 : 1, has_changes ? 24 : 48,  has_changes ? 12 : 18);

  int name_width = strlen(hex->name);
  if(name_width > 29) {
      int paren_index = strchr(hex->name,'(') - hex->name;
      if(paren_index > 0) 
        name_width = paren_index - 1;
  }

  printf("%4d. %*s",hex->number,name_width,hex->name);
  if(has_changes) {
    if(name_width < 29)
      printf("%*s",(int)(29 - strlen(hex->name))," ");
    hex = get_hexagram(secondary_num);
    printf("==>   %d. %-29s",hex->number,hex->name);
  }
  printf("\n");

  wait_for_user(true);

  print_hexagram_info(num);

  if(has_changes) {
    wait_for_user(true);
    print_emphasized("THE CHANGES:\n\n");
    print_analaysis_of_changing_lines(user_lines);
    printf("\nRelating hexagram:\n\n");
    draw_multiple_user_lines(&cast_user_lines[1], 1,  24,  12);
    print_hexagram_info(secondary_num);
  }
}

static void print_hexagram_info(int number) { // note that this is 1-indexed for compatibility with humans / the texts
  const i_ching_hexagram *hex = get_hexagram(number); // whereas this is 0-indexed
  printf("\n%s\n\n",hex->name);
  printf("%s\n",hex->cm);
  wait_for_user(true);
  print_emphasized("THE JUDGEMENT:");
  printf("\n\n%s\n\n",hex->jd);
  printf("%s\n",hex->j_cm);
  wait_for_user(true);
  print_emphasized("THE IMAGE:");
  printf("\n\n%s\n",hex->im);
  printf("\n%s\n\n",hex->i_cm);
}

/*
static void display_intro() {
    int n_hex = 0;
    char test_user_lines[12][7]; // memset
    for(int i = 0; i < 64; i++) {
    char *li = test_user_lines[n_hex];
    if(i & 1) li[0] = '9';
    if(i & 2) li[1] = '9';
    if(i & 4) li[2] = '9';
    if(i & 8) li[3] = '9';
    if(i & 16) li[4] = '9';
    if(i & 32) li[5] = '9';
    if(++n_hex > 8) {
      draw_multiple_user_lines(test_user_lines, 10, 8, 6);
      printf("\n\n");
      n_hex = 0;
    }
  }
}
*/

