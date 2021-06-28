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