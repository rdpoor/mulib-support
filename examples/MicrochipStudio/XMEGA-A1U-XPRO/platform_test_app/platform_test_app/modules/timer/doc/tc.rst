======================
TC driver
======================
It consists of a base counter and a set of compare channels. The base counter can be used to count clock cycles or events, or let events control how it counts clock cycles. 
It has direction control and period setting that can be used for timing. The compare channels can be used together with the base counter to do compare match control,
frequency generation, and pulse width waveform modulation. 

The timer/counter is clocked by the peripheral clock with optional prescaling or from the event system.

The timer/Counter has a split mode feature that splits it into two 8-bit timer/counters with four compare channels each.

NOTE
----
Some of the Xmega_D family devices that has timer/counters have extensions to enable more specialized waveform and frequency generation. 
The advanced waveform extension (AWeX) is intended for motor control and other power control applications. 
It enables low- and highside output with dead-time insertion, as well as fault protection for disabling and shutting down external drivers. 
It can also generate a synchronized bit pattern across the port pins. 

The high-resolution (hi-res) extension can be used to increase the waveform output resolution by four or eight times by using an internal clock source running up to four times
faster than the peripheral clock.

NOTE (TC4/TC5 Driver)
---------------------
There are two differences between timer/counter type 4 and type 5. Timer/counter 4 has four CC channels, and timer/counter 5 has two CC channels. All information related to CC channels 3 and 4 is valid only for timer/counter 4.
Both timer/counter 4 and 5 can be in 8-bit mode, allowing the application to double the number of compare and capture
channels that then get 8-bit resolution.

Features
--------
* Initialization

Applications
------------
* Frequency Generation
* Single-slope PWM (pulse width modulation)
* Dual-slope PWM
* Count on event
* Quadrature decoding

Dependencies
------------
* CLK for clock
* CPUINT/PMIC for Interrupt
* EVSYS for events
* UPDI/PDI/JTAG for debug
* PORT for Waveform Generation

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

