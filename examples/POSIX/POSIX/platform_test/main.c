#include "platform_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <mulib.h>
#include "mu_platform.h"

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
