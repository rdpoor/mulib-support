# Developer Notes for mulib-support

## 20210526-0659 Untangle #include and -I paths

The requirements for -I paths could be better, perhaps something like this:

* `-I<path_to_mulib>/core` for code that uses any mulib core modules
* `-I<path_to_mulib>/extras` for code that uses any mulib extras modules
* `-I<path_to_demos/shared` for demo apps
* `-I<path_to_platform>` for code that uses platform specific declarations

This means that:

Files in mulib/core can freely refer to each other, e.g. `mu_sched.c` may use
`#include "mu_task.h"`.

`mu_task.h` will have `#include mu_time.h`, which will be found in
`platform/mu_time.h`.

Any demo app that needs `mu_stddemo` functions will `#include "mu_stddemo.h"`.

Files in mulib/extras can easily reference declarations in muilb/core.


### But what about initialization?

If an app uses mu_time, it needs to call `mu_time_init()`.  Similarly, if an app
uses stddemo, it needs to call `mu_stddemo_init()`.  Where should that happen?

The best I can think of is as follows:

If your code includes `foo.h`, it should also call `foo_init()` at startup.
If `foo_init()` is sensitive to being called multiple times, it can use this
pattern:

```
// file: foo.c

static bool s_initialized = false;

void foo_init() {
    if (s_is_initialized) return;
    ... do regular initialization ...
    s_initialized = true;  
}
```
AM: There's also the problem of needing to call init functions in the right order.

## 20210526-0800 Prune unused / abandoned mulib code

There's a bunch of files that are ripe for pruning or fixing:

* mu_pstore and mu_vect have overlapping functionality.  (NB: mu_log uses mu_vect)
* mu_cirq and mu_spsc have overlapping functionality.
* mu_queue provides functionality similar to mu_vect, but using lists.
* mu_fsm is not well designed and not currently used by any code.
* mu_timer should perhaps migrate to ../extras
* mu_log could (should) use thunks.

## 20210620-1800 Checklist for Release Candidate

Prior to pushing develop-rc to main:

* Replace all absolute paths in example apps with relative paths
* Verify all example apps run and work as advertised
* Provide short README.md for each example app
* [maybe]: Only include core modules that are used in example apps
* Create a matrix of which modules are used in which apps


## 20210623-1000 Andy Topics

Working on the POSIX stuff has led me to think about:

* the name "stddemo" -- it seems counterintuitive that I should #include anything with "demo" in its name as part of an application
** how about breaking stddemo up into mu_button_io, mu_led_io, mu_terminal_io (mu_serial_io?)
* let's talk through the initialization regime.   mu_platform_init(), mu_stddemo_init(), mu_time_init(), mu_schedule_init(), mulib_init()
* sexier modularity + Makefiles.   Would be great if the platform-specific makefile complexity could be siloed so that if you're making an app, your Makefile is much simpler.   
** should mu_platform have its own Makefile?   Should we build modules?
** Would be great to have a simple mechanism for cherry-picking modules from core, extras.   In lieu of CMake, maybe some shell scripts?   -D flags?
* Why are .h files living alongside .c files?   Are Terry and I old fogies?   If during a port you're meant to change anything in a .h, it should be stand alone, like config.h.  Also centralize / factor the platform changes.
* Why is mulib.c outside of core/?
* There's redundnancy with ansi routines in tower and POSIX/mu_stddemo.   Pull up into mu_platform/mu_terminal_io?

