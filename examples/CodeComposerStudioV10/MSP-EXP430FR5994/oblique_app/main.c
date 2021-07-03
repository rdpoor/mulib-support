#include "oblique.h"

/**
 * main.c
 */
int main(void)
{
    oblique_init(true);
    while(1) {
        oblique_step();
    }
}
