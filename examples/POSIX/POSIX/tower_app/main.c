#include "tower.h"
/**
 * main.c
 */

int main(void)
{
    tower_init(); // passing in false means we dont wait for a button press, we will run the oblique_choose_and_print_task right away
    while(1) {
        tower_step();
    }
}
