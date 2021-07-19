#include "tower.h"
/**
 * main.c
 */

int main(void)
{
    tower_init();
    while(1) {
        tower_step();
    }
}
