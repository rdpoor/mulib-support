#include "wall_clock.h"

/**
 * main.c
 */

int main(void)
{
    wall_clock_init(); // this will hang until a key press ir button press appens
    while(1) {
        wall_clock_step();
    }
}
