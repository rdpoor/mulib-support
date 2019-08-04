# ulib
ulib is curated collection of modules specifically for microcontrollers and
other resource-constrained environments.

## About ulib

ulib is a collection of software modules, written in C, designed to run on
microcontrollers and other resource-constrained systems.

ulib modules strive towards the following design goals.  (Whether or not they
always succeed is an open question):

* Self-contained: minimize dependencies on external libraries
* Two-file implementation: whenever practical, ulib modules are embodied in one
  header file and one C file.
* Tested: unit tests validate the API
* Documented API: The API is clearly documented in the header files.

## The modules

### bcast

Low-overhead publish / subscribe message passing.

### chron

Functions to manipulate time values, properly handling roll-over

### jemi

Build complex JSON structures and emit them as JSON formatted strings.

### heapsort

O(log N) in-place sorting of pointer-sized objects.

### pool

Allocate and free fixed-sized objects in a pool

### sched

Time agnostic, single-thread, run-to-completion scheduler

### mulog

No-nonsense logging
