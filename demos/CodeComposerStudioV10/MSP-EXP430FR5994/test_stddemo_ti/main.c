#include "test_stddemo.h"

/**
 * main.c
 */
int main(void)
{
    test_stddemo_init();
    while(1) {
        test_stddemo_step();
    }
}
