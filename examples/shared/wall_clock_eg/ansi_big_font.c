/**
 * ansi_big_font.c
 *
 *
 */

// =============================================================================
// Includes
#include <stdio.h>
#include <string.h>
#include <math.h>
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
};


// =============================================================================
// Local (forward) declarations
static int big_font_index_for_char(char c);
int byte_dict_index_of(char c);
static void test_decomp(char *dict, uint64_t longs[]);
int bitsFromStorage(uint64_t store[], int bit_index, int bit_count);
void bitsToStorage(uint64_t store[], int bit_index, int bit_count, int value);

// =============================================================================
// Public code


void print_string_using_big_font(char *wut) {
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
  if(c < 48 || c > 58) return 10; // to avoid a crash
  return c - 48;
}




// compression experiments

#define WORD_BITS 64
#define MAX_WORD_STORAGE 4 // MAX_WORD_STORAGE * WORD_BITS is the total bits available per string
#define COUNT_BITS 16

// NOTE this store is just used for producing the compressed strings, not needed for consumption / decoding
static char byte_dict[128];
static uint64_t compressor_store[MAX_WORD_STORAGE]; 

int bitsFromStorage(uint64_t store[], int bit_index, int bit_count) {
   // which uint64_t word contains our data
    uint64_t store_index = bit_index / WORD_BITS; // rounded down
    uint64_t stored_val = store[store_index];
   // where within the word our data is stored
    int word_index = WORD_BITS - (bit_index % WORD_BITS) - bit_count;
    //printf("bitsFromStorage stored_val %llu word_index %d mask %d\n",stored_val,word_index,((1 << bit_count) - 1));
   // printf("(stored_val >> word_index) %llu\n",(stored_val >> word_index));
    int answer = (stored_val >> word_index) & ((1 << bit_count) - 1);
    return answer;
}

void bitsToStorage(uint64_t store[], int bit_index, int bit_count, int value) {
    uint64_t longValue = value;
    // which  word we will store into
    uint64_t store_index = bit_index / WORD_BITS; // rounded down
    uint64_t stored_val = store[store_index];
    //printf("existing stored_val %llu\n",stored_val);
   // where within the word our data is stored
    int word_index = WORD_BITS - (bit_index % WORD_BITS) - bit_count;
    stored_val += (longValue << word_index);
    store[store_index] = stored_val; // write it back to store
    //printf("wrote %d %d %d -- %llu %llu %llu\n",bit_index,bit_count,value, stored_val,  (longValue << word_index), stored_val >> word_index);
}








char *test_comp(char * s) {
  // build the byte dict
  memset(byte_dict,0,128);
  for(int i=0; i < strlen(s); i++) {
    char c = s[i];
    int byte_dict_index = byte_dict_index_of(c);
    if(byte_dict_index == -1)
      byte_dict[strlen(byte_dict)] = c;
  }

  uint8_t nbits = log2(strlen(byte_dict)) + 1; // +1 ????
  //printf("strlen(byte_dict) %lu nbits %d\n\n",strlen(byte_dict), nbits);

  printf("char *original = \"%s\";\n",s);
  printf("char *byte_dict = \"%s\";\n",byte_dict);

  // encode
  int word_index = 0;

  // first store the count
  bitsToStorage(compressor_store, word_index, COUNT_BITS, strlen(s));
  word_index += COUNT_BITS;

  // now for each character, get its index in the dict, and write that to bit storage
  for(int i=0; i < strlen(s); i++) {
    char c = s[i];
    int ci = byte_dict_index_of(c); // turn the char into the small int index into the dict
    bitsToStorage(compressor_store, word_index, nbits, ci);
    word_index += nbits;
  }

  // dump compressed storage in c code to the console

  printf("uint64_t compressed_string[] = {");

  for(int i = 0; i < MAX_WORD_STORAGE; i++) {
    printf("%lluull",compressor_store[i]);
    if(i < MAX_WORD_STORAGE - 1)
      printf(",");
  }
  printf("};\n\n");

  return NULL;
}

int byte_dict_index_of(char c) {
   for(int i=0; i < strlen(byte_dict); i++) {
    if(byte_dict[i] == c) return i;
  }
  return -1;
}



static void test_decomp(char *dict, uint64_t store[]) {
  uint16_t n_chars = bitsFromStorage(store, 0, COUNT_BITS);
  printf("n_chars %d\n",n_chars);
  uint8_t character_index = 0;
  int bit_index = COUNT_BITS;
  uint8_t nbits = log2(strlen(byte_dict)) + 1; // +1 ????

  printf("ANSWER:\n");

  while(character_index < n_chars) {
    int ci = bitsFromStorage(store, bit_index, nbits);
    char aChar = dict[ci];
    if(aChar == 0) break;
    printf("%c",aChar);
    character_index++;
    bit_index += nbits;
  }
  printf("\n");
}

void try2(void) {

  printf("store test\n");
  bitsToStorage(compressor_store, 0, COUNT_BITS, 37);
  bitsToStorage(compressor_store, COUNT_BITS, 3, 5);

  int ans = bitsFromStorage(compressor_store, 0, COUNT_BITS);
  printf("ans %d\n", ans);
  ans = bitsFromStorage(compressor_store, COUNT_BITS, 3);
  printf("ans %d\n", ans);
}

void try(void) {
  char *byte_dict = " _/\\|";
  uint64_t char0[] = {13511425947865280ull,9369774767598502424ull,2309381787387691008ull}; 
  test_decomp(byte_dict, char0);
}


void try3(void) {

char *original = "  ___    / _ \\  | | | | | | | | | |_| |  \\___/  "; // 48 bytes
char *byte_dict = " _/\\|";
uint64_t compressed_string[] = {13511425947865280ull,9369774767598502424ull,2309381787387691008ull}; // 24 bytes


  printf("original: %s\n",original);

  test_decomp(byte_dict, compressed_string);
}

