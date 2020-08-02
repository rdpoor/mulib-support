#include <atmel_start.h>
#include "mv_vm_test.h"

int main(void)
{
    /* Initializes MCU, drivers and middleware */
    atmel_start_init();
    mv_vm_test_init();

    /* Replace with your application code */
    while (1) {
        mv_vm_test_step();
    }
}
