#include "platform_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <mulib.h>
#include "kbd_read.h"

/**
 * main.c
 */

int main(void)
{
    printf("FR\n");
    start_kbd_reader_thread();
    platform_test_init(); 
    while(1) {
        platform_test_step();
    }
}
