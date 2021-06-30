#include "morse_2.h"
#include <stdio.h>
#include <stdlib.h>
#include <mulib.h>

/**
 * main.c
 */

int main(void)
{
    morse_2_init(); // this will hang until a key press ir button press appens
    while(1) {
        morse_2_step();
    }
}
