======================
PMIC driver
======================
XMEGA devices contain a Programmable Multilevel Interrupt Controller (PMIC) for Interrupt prioritizing according to level and vector address
PMIC have three selectable interrupt levels for all interrupts: low, medium and high and supports selectable, round-robin priority scheme within low-level interrupts

Features
--------
* Initialization

Applications
------------
* Complex application

Dependencies
------------
* CLK for clock

Note
----
* ISR will be generated only when Global Interrupt checkbox and driver_isr Harness checkbox are enabled

Concurrency
-----------
N/A

Limitations
-----------
N/A

Knows issues and workarounds
----------------------------
N/A