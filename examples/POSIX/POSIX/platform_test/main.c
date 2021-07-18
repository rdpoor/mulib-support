#include "platform_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <mulib.h>
#include "mu_ansi_term.h"
/**
 * main.c
 */

int main(void)
{
    mu_ansi_term_clear_screen();
    platform_test_init(); 
    while(1) {
        platform_test_step();
    }
}
