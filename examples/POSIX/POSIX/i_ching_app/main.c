#include "i_ching.h"
#include <stdio.h>
#include <stdlib.h>
#include <mulib.h>

/**
 * main.c
 */

int main(void)
{
    i_ching_init(); // in this example, everything happens in here
    while(1) {
        i_ching_step();
    }
}
