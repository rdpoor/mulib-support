# mulibDemo

mulibDemo is a demonstrattion of the mulib framwork, focusing on the mulib
scheduler, timed tasks, interrupt handling and time manipulation.

mulib is designed to be easily ported to different archictures; this demo is
built for the Atmel/Microchip SAMD21 XPlained development board, built using
Atmel START and Atmel Studio 7.

mulibDemo is modeled closely after the FreeRTOSDemo, which is available as a
sample project in Atmel START.  One goal of mulibDemo is to highlight how a
FreeRTOS project can be re-cast as a mulib project.

## Running mulibDemo

### Prerequisites

1. Atmel Studio 7 (available for free download at )
2. An Atmel ATSAMD21 XPlained development board (e.g. )
3. A Terminal Emulator, such as Tera Term or CoolTerm.

### To Launch

1. Connect SAMD21 XPlained board to your computer via USB cable
2. Launch your Terminal Emulator, configure for 57600 Baud.
3. Double click on the mulibDemo.atsln file to launch Studio 7
4. In Studio 7, click on Debug => Start Without Debugging

### To Observe

mulibDemo will update the screen every 5 seconds, showing the state of the
three tasks.  (Note: we may include the Idle task as a fourth task in a
future release.)  Each task name is followed by a *State* letter:

* A for active, the currently running task
* I for idle, i.e. not presently scheduled to run
* P for pending, scheduled for some point in the future
* R for runnable, scheduled and ready to run

Each task also shows a *Calls* field indicating how many time the task has been
called since initialization.

The *Max Latency* field shows the maximum amount of time the task has spent
running.  Since mulib uses a run-to-completion scheduler, this number defines
the maximum latency you can expect between tasks.

The *Runtime* field shows the accumulated time spend running that task.

## Developer Notes

* Make the Idle Task be a first-class Task object so we can display how much
time was spent running it, # of calls, etc.

* Create a follow-on version that sleeps the processor rather than run the
idle task.  This would be the basis for a very low power system, and would
port to multiple architectures easily.

* Upgrade mu_bcast to manipulate mu_task objects.

* When the baud rate was set to 115200, the output was garbled.  It would be
useful to understand where the problem lies.

* Port to other boards.  [SAME54, SAML22, XMEGA A1U] XPlained boards would be
a good start.
