#include "oblique.h"
#include <stdio.h>
#include <stdlib.h>
#include <mulib.h>

/**
 * main.c
 */

int main(void)
{
    oblique_init(); // this will hang until a key press ir button press appens
    while(1) {
        oblique_step();
    }
}
