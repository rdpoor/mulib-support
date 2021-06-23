#include "test_stddemo.h"
#include <stdio.h>
#include <stdlib.h>
#include "mulib.h"
#include "mu_stddemo.h"

/**
 * main.c
 */

void update_display();

/**
 * @brief
 * gets called when anyone calls set_led()
 * since there's no obvious led here in posix, we use this as a prompt to make some other visual change
 * in this case, we simply clear the line and change the text color randomly
 * 
*/

void led_was_changed(bool on) {
    //printf("led_was_changed %d",on);
    update_display();
    // if(on) {
    //     terminal_erase_last_line();
    //     set_terminal_foreground_color(mu_random_range(1,230));
    // }
}

void update_display() {
    terminal_clear();
    set_terminal_foreground_color(get_led() ? 2 : 8);
    printf("•\n");
    terminal_reset();
}

int main(void)
{
    int ch;
    set_terminal_foreground_color(3);
    set_led_callback(&led_was_changed);
    test_stddemo_init(); 
    mu_set_terminal_attributes(false, false, false); // lets us read individual keypresses from the terminal, without hanging
    while(1) {
        test_stddemo_step();
        ch = mu_get_key_press(); // because of our call to mu_set_terminal_attributes() above, this wont hang -- will return the next character on stdin or 0 if there's nothing waiting
        switch(ch) {
            case 'q':
                exit(0);
                break;
            default:
                if(ch > 0) {
                    test_stddemo_x(true);
                    //s_button_was_pressed = true;
                }
                break;
        }
    }
}
