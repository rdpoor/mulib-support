#include "morse_3.h"


/**
 * main.c
 */

int main(void)
{
    morse_3_init(); // this will hang until a key press ir button press appens
    while(1) {
        morse_3_step();
    }
}
