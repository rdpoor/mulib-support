/**
 * ansi_big_font.c
 *
 *
 */

// =============================================================================
// Includes
#include <stdio.h>
#include <string.h>
#include "fb.h"

// =============================================================================
// Local types and definitions


// =============================================================================
// Local storage

static int big_font_line_count = 6;
// star wars font from https://manytools.org/hacker-tools/ascii-banner/
const char *big_font[] = { // note that '\' has to be escaped as "\\"
  "  ___    / _ \\  | | | | | | | | | |_| |  \\___/  ", // 0 (48)
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
  "                 _  (_)    ", // .
};


// =============================================================================
// Local (forward) declarations
static int big_font_index_for_char(char c);

// =============================================================================
// Public code


void print_string_using_big_font(char *wut) {
  fb_clear();
  for(int i = 0; i < big_font_line_count; i++) {
    char *fb_line_start = fb_row_ref(i);
    if(!fb_line_start) {
      printf("print_string_using_big_font() needs fb_init(width, height) to have been called.\n");
      return;
    }
    for(int ci = 0; ci < strlen(wut); ci++) {
      char c = wut[ci];
      const char *s = big_font[big_font_index_for_char(c)];
      int line_len = strlen(s) / big_font_line_count;
      sprintf(fb_line_start, "%.*s ", line_len, s + (line_len * i));
      fb_line_start += line_len + 1;
    }
    // we clear the line all the way to the end, to make sure that we erase anything left over from wider characters printed previously
    fb_clear_to_end_of_line(fb_line_start);
  }
  fb_flush();
}



// =============================================================================
// Local (static) code

static int big_font_index_for_char(char c) {
  if(c == ':') return 10;
  if(c < 48 || c > 58) return 11; // any chars not currently defined will display as "."
  return c - 48;
}
