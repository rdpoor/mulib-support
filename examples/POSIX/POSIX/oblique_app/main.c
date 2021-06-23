#include "oblique_eg.h"
#include "strategies.h"
#include <stdio.h>
#include <stdlib.h>
#include "mu_stddemo.h"

/**
 * main.c
 */

/**
 * @brief
 * gets called when anyone calls set_led()
 * since there's no obvious led here in posix, we use this as a prompt to make some other visual change
 * in this case, we simply clear the line and change the text color randomly
 * 
*/
#define CSI "\e["


void ansi_term_cursor_position(uint8_t row, uint8_t col) {
  // optimize.
  if (row == 0) {
    if (col == 0) {
      puts(CSI "H");
    } else {
      printf(CSI ";%dH", col+1);
    }
  } else {
    if (col == 0) {
      printf(CSI "%dH", row+1);
    } else {
      printf(CSI "%d;%dH", row+1, col+1);
    }
  }
}


void led_was_changed(bool on) {
    if(!on) {
        //terminal_erase_last_line();
        //terminal_erase_last_line();
        terminal_clear();
        set_terminal_foreground_color(mu_random_range(1,230));
    }
}

void led_was_changed2(bool on) {
    char *choice;
    if(on) {
        //terminal_erase_last_line();
       choice = current_choice();
        //terminal_clear();
       // printf("choice %s",choice);
        set_terminal_foreground_color(mu_random_range(1,230));
        //ansi_term_cursor_position(strlen(choice),10);
    }
}


int main(void)
{
    int ch;
    set_terminal_foreground_color(3);
    printf("Wait patiently, hit a key for a new idea, or hit 'q' to quit:\n%s%s\n",ANSI_ESC,ANSI_INVERSE);
    set_led_callback(&led_was_changed);
    oblique_eg_init(false); // passing in false means we dont wait for a button press, we will run the oblique_choose_and_print_task right away
    mu_set_terminal_attributes(false, false, false); // lets us read individual keypresses from the terminal, without hanging
    while(1) {
        oblique_eg_step();
        ch = mu_get_key_press(); // because of our call to mu_set_terminal_attributes() above, this wont hang -- will return the next character on stdin or 0 if there's nothing waiting
        switch(ch) {
            case 'q':
                exit(0);
                break;
            default:
                if(ch > 0) {
                    mu_sched_task_now(oblique_choose_and_print_task());
                }
                break;
        }
    }
}
