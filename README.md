# mulib - Micro Library
mulib is curated collection of software modules written in C specifically
designed for microcontrollers and other resource-constrained environments.

## mulib's design philosophy

mulib modules strive towards the following design goals.

* Self-contained: minimize dependencies on external libraries
* Two-file implementation: whenever practical, mulib modules are embodied in one
  header file and one C file.
* Tested: unit tests validate the API
* Documented API: The API is clearly documented in the header files.
* Minimal Safety: mulib favors minimizing time and code space over argument
  validation.
* User controls memory management: Modules that require dynamic allocation and
  freeing of objects take user-defined malloc() and free() methods in the
  module's init() function.

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

Clock agnostic, single-thread, run-to-completion scheduler

### mulog

No-nonsense logging
