// hack 
static uint8_t _cursor_x, _cursor_y;

void mu_ansi_term_save_cursor_position2(void) {
  char buf[30]={0};
  int ret, i, pow;
  char ch;

 
  _cursor_x = _cursor_y = 0;

  mu_ansi_term_enter_noncanonical_mode();


  printf(MU_ANSI_TERM_ESC "6n");

for( i = 0, ch = 0; ch != 'R'; i++ )
 {
    ret = read(0, &ch, 1);
    if ( !ret ) {
       fprintf(stderr, "getpos: error reading response!\n");
      mu_ansi_term_exit_noncanonical_mode();
       return;
    }
    buf[i] = ch;
 }

 if (i < 2) {
    printf("i < 2\n");
    mu_ansi_term_exit_noncanonical_mode();
    return;
 }

 for( i -= 2, pow = 1; buf[i] != ';'; i--, pow *= 10)
     _cursor_x = _cursor_x + ( buf[i] - '0' ) * pow;

 for( i-- , pow = 1; buf[i] != '['; i--, pow *= 10)
     _cursor_y = _cursor_y + ( buf[i] - '0' ) * pow;

   mu_ansi_term_exit_noncanonical_mode();
} 

void mu_ansi_term_restore_cursor_position2(void) {
  mu_ansi_term_set_cursor_position(_cursor_x,_cursor_y);
} 




/* Makefile a la carte testing

# alacarte: $(SRC_MULIB_CORE_DIR)/mu_sched.o $(SRC_MULIB_CORE_DIR)/mu_dlist.o $(SRC_MULIB_CORE_DIR)/mu_thunk.o $(SRC_MULIB_CORE_DIR)/mu_spsc.o $(SRC_MULIB_CORE_DIR)/mu_task.o $(SRC_MU_PLATFORM_DIR)/mu_time.o $(SRC_MU_PLATFORM_DIR)/mu_rtc.o $(SRC_MULIB_EXTRAS_DIR)/mu_ansi_term.o $(OBJECTS_WALL_CLOCK_EG) main.o
#   $(CC) $(SRC_MULIB_CORE_DIR)/mu_sched.o $(SRC_MULIB_CORE_DIR)/mu_dlist.o $(SRC_MULIB_CORE_DIR)/mu_thunk.o $(SRC_MULIB_CORE_DIR)/mu_spsc.o $(SRC_MULIB_CORE_DIR)/mu_task.o $(SRC_MU_PLATFORM_DIR)/mu_time.o $(SRC_MU_PLATFORM_DIR)/mu_rtc.o $(SRC_MULIB_EXTRAS_DIR)/mu_ansi_term.o $(OBJECTS_WALL_CLOCK_EG) main.o -o $(WALL_CLOCK_EG)


*/

/ Implementation of itoa()
char* itoa(int num, char* str, int base)
{
    int i = 0;
    bool isNegative = false;
  
    /* Handle 0 explicitely, otherwise empty string is printed for 0 */
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
  
    // In standard itoa(), negative numbers are handled only with 
    // base 10. Otherwise numbers are considered unsigned.
    if (num < 0 && base == 10)
    {
        isNegative = true;
        num = -num;
    }
  
    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9)? (rem-10) + 'a' : rem + '0';
        num = num/base;
    }
  
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
  
    str[i] = '\0'; // Append string terminator
  
    // Reverse the string
    reverse(str, i);
  
    return str;
}











// compression experiments
#include <math.h>

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
    int answer = (stored_val >> word_index) & ((1 << bit_count) - 1);
    return answer;
}

void bitsToStorage(uint64_t store[], int bit_index, int bit_count, int value) {
    uint64_t longValue = value;
    // which  word we will store into
    uint64_t store_index = bit_index / WORD_BITS; // rounded down
    uint64_t stored_val = store[store_index];
   // where within the word our data is stored
    int word_index = WORD_BITS - (bit_index % WORD_BITS) - bit_count;
    stored_val += (longValue << word_index);
    store[store_index] = stored_val; // write it back to store
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



static void test_decomp(char *dict, uint64_t store2[]) {
  uint16_t n_chars = bitsFromStorage(store2, 0, COUNT_BITS);
  printf("n_chars %d\n",n_chars);
  uint8_t character_index = 0;
  int bit_index = COUNT_BITS;
  uint8_t nbits = log2(strlen(dict)) + 1; // +1 ????

  printf("ANSWER:\n");

  while(character_index < n_chars) {
    int ci = bitsFromStorage(store2, bit_index, nbits);
    char aChar = dict[ci];
    if(aChar == 0) break;
    printf("%c",aChar);
    character_index++;
    bit_index += nbits;
  }
  printf("\n");
}

void testBitStorage(void) {

  printf("store test\n");
  bitsToStorage(compressor_store, 0, COUNT_BITS, 37);
  bitsToStorage(compressor_store, COUNT_BITS, 3, 5);

  int ans = bitsFromStorage(compressor_store, 0, COUNT_BITS);
  printf("ans %d\n", ans);
  ans = bitsFromStorage(compressor_store, COUNT_BITS, 3);
  printf("ans %d\n", ans);
}

void try(void) {
  char *byte_dict2 = " _/\\|";
  uint64_t char0[] = {13511425947865280ull,9369774767598502424ull,2309381787387691008ull}; 
  test_decomp(byte_dict2, char0);
}










/* Example of using system() dynamically by doing sprintf to a string that we pass to system()

*/

char *datafile = "file.dat";
  char *sortedfile = "file.sort";
  char cmdbuf[50];
  FILE *fp = fopen(datafile, "w");

  /* ...write to fp to build data file... */

  fclose(fp);

  sprintf(cmdbuf, "sort < %s > %s", datafile, sortedfile);
  system(cmdbuf);

  fp = fopen(sortedfile, "r");
  /* ...now read sorted data from fp... */

