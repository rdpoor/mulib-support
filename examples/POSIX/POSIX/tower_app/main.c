#include "tower.h"
#include <stdio.h>
#include <stdlib.h>
#include "mu_stddemo.h"

/**
 * main.c
 */

int main(void)
{
    int ch;
    tower_init(); // passing in false means we dont wait for a button press, we will run the oblique_choose_and_print_task right away
    mu_set_terminal_attributes(false, false, false); // lets us read individual keypresses from the terminal, without hanging
    while(1) {
        tower_step();
        ch = mu_get_key_press(); // because of our call to mu_set_terminal_attributes() above, this wont hang -- will return the next character on stdin or 0 if there's nothing waiting
        switch(ch) {
            case 'q':
                exit(0);
                break;
            default:
                if(ch > 0) {
                }
                break;
        }
    }
}
