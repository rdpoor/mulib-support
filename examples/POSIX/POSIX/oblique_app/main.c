#include "oblique_eg.h"
#include <stdio.h>
#include <stdlib.h>
#include "mu_stddemo.h"

/**
 * main.c
 */


/**
 * @brief we register for this callback because the example code toggles the led each time it makes a new choice
 * which gives us this opportunity to clear the screen and set a new random color for the new choice
 */

void on_led_change(bool on) {
    if(on) { 
        terminal_clear();
        set_terminal_foreground_color(mu_random_range(1,230));
    } 
}

int main(void)
{
    int ch;
    set_terminal_foreground_color(3);
    printf("Wait patiently, hit a key for a new idea, or hit 'q' to quit:\n%s%s\n",ANSI_ESC,ANSI_INVERSE);
    set_led_callback(&on_led_change);
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
