/*
 * platform_test.c
 *
 *  Created on: Apr 18, 2021
 *      Author: r
 */

// =============================================================================
// Includes

#include "wall_clock.h"
#include <mulib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "mu_platform.h"

// =============================================================================
// Local types and definitions


// =============================================================================
// Local (forward) declarations

static void button_cb(uint8_t button_id, bool button_is_pressed);

// =============================================================================
// Local storage

// note that '\' has to be escaped as "\\"
static int big_font_line_count = 6;
// star wars font from https://manytools.org/hacker-tools/ascii-banner/
const char *big_font[] = {
  "  ___    / _ \\  | | | | | | | | | |_| |  \\___/  ", // 0
  " __  /_ |  | |  | |  | |  |_| ", // 1
  " ___   |__ \\     ) |   / /   / /_  |____| ", // 2
  " ____   |___ \\    __) |  |__ <   ___) | |____/  ", // 3
  " _  _    | || |   | || |_  |__   _|    | |      |_|   ", // 4
  " _____  | ____| | |__   |___ \\   ___) | |____/  ", // 5
  "   __     / /    / /_   | '_ \\  | (_) |  \\___/  ", // 6
  " ______  |____  |     / /     / /     / /     /_/     ", // 7
  "  ___    / _ \\  | (_) |  > _ <  | (_) |  \\___/  ", // 8
  "  ___    / _ \\  | (_) |  \\__, |    / /    /_/   ", // 9
  "     _  (_)      _  (_)    ", // :
  "    _ (_)    _ (_)" // :
};

int big_font_index_for_char(char c) {
  if(c == ':') return 10;
  return c - 48;
}

void print_string_using_big_font(char *wut) {
  for(int i = 0; i < big_font_line_count; i++) {
    for(int ci = 0; ci < strlen(wut); ci++) {
     char c = wut[ci];
      const char *s = big_font[big_font_index_for_char(c)];
      int line_len = strlen(s) / big_font_line_count;
      printf("%.*s ", line_len, s + (line_len * i)); 
    }
    printf("\n");
  }
}

volatile static bool s_button_was_pressed;

// =============================================================================
// Public code

void wall_clock_init(void) {
  print_string_using_big_font("67:89");
  mulib_init();
  //mu_platform_init();
  mu_button_io_set_callback(button_cb);

  mu_ansi_term_clear_screen();
  mu_ansi_term_set_foreground_color(MU_ANSI_TERM_YELLOW);
  mu_ansi_term_set_cursor_visible(false);

  
  //mu_begin_polling_for_keypress();
  atexit(mu_ansi_term_exit_noncanonical_mode); // restores terminal attributes
}

void wall_clock_step(void) {
  mu_sched_step();
  unsigned char kp = mu_term_get_current_keypress();
  if(kp == 'q') exit(0);
}

// =============================================================================
// Local (private) code

static void button_cb(uint8_t button_id, bool button_is_pressed) {
  (void)button_id;

  if (button_is_pressed) {
    s_button_was_pressed = true;
  }
}
