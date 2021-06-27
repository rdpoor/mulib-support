#include "platform_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <mulib.h>

/**
 * main.c
 */

void update_display();

/**
 * @brief
 * gets called when anyone calls set_led()
 * since there's no obvious led here in posix, we use this as a prompt to make some other visual change
 * in this case, we clear the terminal and print a colored dot 
 * 
*/


/**
 * @brief Restore the terminal to its original state
 */


void on_led_change(bool on) {
    update_display();
}

void update_display() {
    mu_ansi_term_clear_screen();
    mu_ansi_term_set_foreground_color(mu_led_io_get(MU_LED_0) ? MU_ANSI_TERM_BRIGHT_GREEN : MU_ANSI_TERM_GREEN);
    printf("•\n");
    mu_ansi_term_reset();
}

int main(void)
{
    int ch;
    mu_ansi_term_set_foreground_color(MU_ANSI_TERM_YELLOW);
    //set_led_callback(&on_led_change);
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
