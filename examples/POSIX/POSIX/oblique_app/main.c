#include "oblique.h"
#include <stdio.h>
#include <stdlib.h>
#include <mulib.h>
#include "kbd_read.h"

/**
 * main.c
 */

int main(void)
{
    start_kbd_reader_thread();
    oblique_init(); // this will hang until a key press or button press happens
    while(1) {
        oblique_step();
    }
}
