#include <atmel_start.h>
#include "mulib_port_test.h"

int main(void)
{
    /* Initializes MCU, drivers and middleware */
    atmel_start_init();
    mulib_port_test_init();

    /* Replace with your application code */
    while (1) {
        mulib_port_test_step();
    }
}
