#include <atmel_start.h>
#include "task_join.h"

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

	/* Replace with your application code */
	task_join_init();
	while (1) {
		task_join_step();
	}
}
