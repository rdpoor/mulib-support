#include "morse_1.h"

/**
 * main.c
 */

int main(void)
{
    morse_1_init(); // this will hang until a key press ir button press appens
    while(1) {
        morse_1_step();
    }
}
