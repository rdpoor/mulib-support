
================================
SLEEP and Power-Reduction driver
================================
Sleep modes are used to shut down peripherals and clock domains in the device in order to save power. The Sleep Controller (SLEEP) controls and handles the transitions between active modes and sleep modes.
Power-Reduction can be used to shutdown each unnecessary peripheral individually.

NOTE: To actually enter sleep, the user need to invoke the sleep instruction.

Features
--------
* Initialization

Applications
------------
* Power saving by controlling the sleep modes.
* Power-Reduction by shutting down unnecessary peripherals.

Dependencies
------------
* CLK for clocks
* UPDI/PDI/JTAG for debug

Concurrency
-----------
N/A

Limitations
-----------
N/A

Knows issues and workarounds
----------------------------
N/A

