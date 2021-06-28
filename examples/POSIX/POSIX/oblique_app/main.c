#include "oblique.h"
#include <stdio.h>
#include <stdlib.h>
#include <mulib.h>
#include "mu_platform.h"

/**
 * main.c
 */

int main(void)
{
    mu_ansi_term_clear_screen();
    oblique_init(true); // this will hang until a key press happens
    atexit(mu_ansi_term_exit_noncanonical_mode); // restores terminal attributes
    mu_ansi_term_clear_screen();
    while(1) {
        oblique_step();
    }
}
