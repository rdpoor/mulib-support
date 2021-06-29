/**
 * ansi_big_font.c
 * 
 * 
 */

// =============================================================================
// Includes

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// =============================================================================
// Local types and definitions


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
