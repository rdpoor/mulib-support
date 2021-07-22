# mulib - A library for microcontrollers  [![Build Status](https://travis-ci.com/rdpoor/mulib.svg?branch=master)](https://travis-ci.com/rdpoor/mulib)

`mulib` is curated collection of software modules written in C, written from the
ground up for microcontrollers and other resource-constrained environments.

`mulib` features:

* a fast and lithe run-to-completion scheduler
* low-overhead timing and event handling
* advanced in-place string handling
* an in-memory publish / subscribe system
* flexible logging
* ... and a variety of other useful modules

## mulib's design philosophy

mulib strives towards the following design goals (though some are evolving):

* **Self-contained, pure C code:** minimize dependencies on external libraries.
* **Two-file implementation:** whenever practical, each module is embodied in
    one header file and one C file.
* **No malloc. Ever.** All of mulib's modules use user-allocated data structures.
* **Well-documented:** API documentation is driven by documentation strings in
    the header files.
* **Well-tested:** unit tests validate the API.
* **Super portable:** One concise mu_vms.c file defines mulib's interface to
    the target environment.
* **Fast and Dangerous:** mulib favors minimizing time and code space over
    argument validation.  But note that `MU_ASSERT()` provides a safety net that
    can be switched on or off at compile time.
* **Single-threaded scheduler:** There are many benefits to a single-threaded
    scheduler -- see [About Run To Completion Schedulers](./RunToCompletion.md)
    for the reasons behind this choice.
* **Low threshold, high ceiling:** A suite of demo programs take you from the
    simplest "blink an LED" example to complex multi-tasking applications.
* **Yours to use:** The entirety of mulib is covered under the MIT open source
    license.

## The modules

### `mu_bitvec`

Bit vector operations on individual bits: set, clear, toggle, test.  Includes
functions that operate on the entire vector.

### `mu_list`

Supports classic singly linked list operations: push, pop, insert, delete,
and a traversal method for user-supplied functions.

### `mu_pstore`

Operations on a collection of pointer-sized objects.  Includes functions for
push, pop, peek, insert, delete, insertion sort and heapsort.

### `mu_queue`

Efficient, extensible first-in, first-out queue operations, built on `mu_list`.

### `mu_sched`

Low-overhead run-to-completion scheduler.  see [About Run To Completion
Schedulers](./RunToCompletion.md) for more information.

### `mu_spsc`

Thread-safe, lock-free, single-producer, single-consumer queue.  It is designed
primarily for use by `mu_sched` to transfer control between interrupt and the
foreground levels, but -- despite its intentionally obscure name -- may be
useful elsewhere.

### `mu_strbuf`

Container for safe read and write access to strings, with methods for exchange
with c-style null-terminated strings.  Companion to mu_substr

### `mu_substr`

Companion to mu_strbuf, implements in-place string manipulation, including slicing,
appending and searching.

### `mu_thunk`

A simple implementation of deferred function calls, with a compile time option
for profiling.  Used extensively by `mu_sched`.

### `mu_time`

Functions to manipulate time values, properly handling roll-over.  Implemented
as a thin interface to platform specific `ports/.../port_time.c`

### `mu_version`

This module simply returns the mulib version number as a string of three
digits, e.g. "1.0.2".

## In the laboratory

These are modules yet to be written and tested (or in some cases, ported from a
previous version of mulib).

### mu_bcast

In-memory publish / subscribe message passing.

### mu_log

Flexible logging with multiple reporting channels and run-time control for
reporting severity for each channel.

### Inline a bunch of functions

Many mulib functions get or set a single field in a structure.  For development,
these have been implemented as standalone functions to simplify debugging and
unit testing, but these functions are candidates for inlining.  E.g.:

```
// file: source/mu_pstore.c
bool mu_pstore_is_empty(mu_pstore_t *pstore) {
  return pstore->count == 0;
}
```
could be recast as:

```
// file: include/mu_pstore.h
static inline mu_pstore_is_empty(mu_pstore_t *pstore) {
  return pstore->count == 0;
}
```

### List-based Single Producer / Single Consumer queue

The initial implementation of spscq uses a fixed-size array as a circular buffer.
In addition, the main scheduling queue is array based and uses insertion sort to
queue deferred tasks.

However, an alternate implementation would include a link field in each task, and
use linked lists for both the spsc queue and for the main scheduling queue.  The
advantage of a list-based system is that the user would not have to pre-declare the
size of the spsc queue and the main scheduling queue.  The disadvantage *might* be
slightly higher scheduler overhead for two reasons:

* Scheduling a task requires a linear search down the list of queued items (but
  the linear search is *very* fast, and we don't anticipate a very deep queue).
* The scheduler must never queue a task twice because the task contains the list
  link, and double-queueing would create a circular list.  So the list must be
  checked each time an event is scheduled (but this could become part of the
  insertion sort process).

### mulib_demo

In order to showcase the main features of mulib (including its portability),
the `mulib_demo` application will:

* Flash an LED periodically
* Print call count, run time, latency etc. for each task
* Demonstrate interrupt handling
* Button push to enter / exit low-power mode

In addition to showing off mulib, it's equally important to show how it can be
ported to various processors and integrated into various IDEs.  Some candidates:

* (Microchip) SAME54 Xplained Pro (Cortex-M4F) + MPLAB X
* (TI) MSP-EXP430GET LaunchPad+ Code Composer
* (STmicro) 32F0308DISCOVERY (Cortex-M0) + IAR Embedded Workbench
* (STmicro) STM32F4DISCOVERY (Cortex-M4) + IAR Embedded Workbench
* (Mcrochip) XMEGA Xplained Pro (AVR) + Atmel Studio 7
* (Kinetis) FRDMKL25Z (Cortex-M0+) + MCUXpresso
* (Espressif) ESP32-DEVKITC-32D (ESP-WROOM-32) + ESP-IDF
