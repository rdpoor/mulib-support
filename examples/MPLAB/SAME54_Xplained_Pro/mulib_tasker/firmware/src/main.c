/*******************************************************************************
  Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This file contains the "main" function for a project.

  Description:
    This file contains the "main" function for a project.  The
    "main" function calls the "SYS_Initialize" function to initialize the state
    machines of all modules in the system
 *******************************************************************************/

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "definitions.h" // SYS function prototypes
#include "mulib_tasker.h"   // mulib_tasker prototypes
#include <stdbool.h>     // Defines true
#include <stddef.h>      // Defines NULL
#include <stdlib.h>      // Defines EXIT_FAILURE

// *****************************************************************************
// *****************************************************************************
// Section: Main Entry Point
// *****************************************************************************
// *****************************************************************************

int main(void) {
  SYS_Initialize(NULL); // initialize MPLAB Harmony modules
  mulib_tasker_init();     // initialize mulib_tasker application

  while (true) {
    SYS_Tasks();      // update MPLAB Harmony module state machines
    mulib_tasker_step(); // update mulib_tasker application state
  }

  /* Execution should not come here during normal operation */

  return (EXIT_FAILURE);
}

/*******************************************************************************
 End of File
*/