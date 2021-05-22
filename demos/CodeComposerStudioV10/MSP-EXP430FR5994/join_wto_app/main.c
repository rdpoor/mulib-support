#include "join_wto_eg.h"

/**
 * main.c
 */
int main(void)
{
    join_wto_eg_init();
    while(1) {
        join_wto_eg_step();
    }
}
