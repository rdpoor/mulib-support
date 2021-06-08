#include "oblique_eg.h"

/**
 * main.c
 */
int main(void)
{
    oblique_eg_init();
    while(1) {
        oblique_eg_step();
    }
}
