# mulibDemo

mulibDemo is a demonstrattion of the mulib framwork, highlighting mulib's
scheduler, timed tasks, interrupt handling and time manipulation.

mulib is designed to be easily ported to different archictures; this demo is
built for the Atmel/Microchip SAMD21 XPlained development board, built using
Atmel START and Atmel Studio 7.

mulibDemo intentionally mimics the behavior of the FreeRTOSDemo, which is
available as a sample project in Atmel START.  One goal of mulibDemo is to
highlight how a FreeRTOS project can be adapted to use the mulib framework.

## Running mulibDemo

### Prerequisites

1. Atmel Studio 7 [available for free download](https://www.microchip.com/mplab/avr-support/atmel-studio-7)
2. A [SAMD21 XPlained development board](https://www.microchip.com/DevelopmentTools/ProductDetails/ATSAMD21-XPRO)
3. A Terminal Emulator, such as [Tera Term](https://osdn.net/projects/ttssh2/)
or [CoolTerm](http://coolterm.org/).

### To Launch

1. Connect SAMD21 XPlained board to your computer via USB cable
2. Launch your Terminal Emulator, configure for 115200 Baud.
3. Double click on the mulibDemo.atsln file to launch Studio 7
4. In Studio 7, click on Debug => Start Without Debugging

### To Observe

mulibDemo will update the screen every 5 seconds, showing the state of the
three tasks.  (Note: we may include the Idle task as a fourth task in a
future release.)  For each task, you will see:
* The **Name** of the task
* The **State** of the task:
  * **A** for active, the currently running task
  * **I** for idle, i.e. not presently scheduled to run
  * **P** for pending, scheduled for some point in the future
  * **R** for runnable, scheduled and ready to run
* A **Calls** field indicating how many time the task has been
called since initialization.
* A **Max Latency** field showing the maximum amount of time the task has spent
running.  Since mulib uses a run-to-completion scheduler, this number defines
the maximum latency you can expect between tasks.
* A **Runtime** field that shows the accumulated time spend running that task.

## Developer Notes

* Make the Idle Task be a first-class Task object so we can display how much
time was spent running it, # of calls, etc.

* Create a follow-on version that sleeps the processor rather than spin-waiting
on the idle task.  This would be the basis for a very low power system, and
would port to multiple architectures easily.

* Upgrade `mu_bcast` to manipulate `mu_task` objects.

* Port to other boards.  [SAME54, SAML22, XMEGA A1U] XPlained boards would be
good candidates.

* Quantify metrics for mulibDemo and FreeRTOSDemo (size, speed, etc).
