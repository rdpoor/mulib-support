#include "platform_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <mulib.h>
#include "mu_platform.h"

/**
 * main.c
 */

int main(void)
{
    int ch;
    mu_ansi_term_set_foreground_color(MU_ANSI_TERM_YELLOW);
    platform_test_init(); 
    mu_ansi_term_enter_noncanonical_mode(); // lets us read individual keypresses from the terminal, without hanging
    atexit(mu_ansi_term_exit_noncanonical_mode); // restores terminal attributes
    while(1) {
        platform_test_step();
        ch = mu_ansi_term_get_key_press(); // because of our call to mu_set_terminal_attributes() above, this wont hang -- will return the next character on stdin or 0 if there's nothing waiting
        switch(ch) {
            case 'q':
                exit(0);
                break;
            default:
                if(ch > 0) {
                    mu_button_io_on_button_press(MU_BUTTON_0);
                }
                break;
        }
    }
}
