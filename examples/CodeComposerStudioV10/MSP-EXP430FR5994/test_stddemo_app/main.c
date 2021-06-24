#include "platform_test.h"

/**
 * main.c
 */
int main(void)
{
    platform_test_init();
    while(1) {
        platform_test_step();
    }
}
