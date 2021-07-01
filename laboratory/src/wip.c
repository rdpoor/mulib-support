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