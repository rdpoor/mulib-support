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

## 20210601-1000 Terry doc notes


3.  It would be cool if I could include just a single .h that I've updated with what I want to do.  For example,
  #define core
if I want to include all the core stuff and
  #define platform
  #define platform_comp1
  #define platform_comp2
if I want to pick-and choose.  Then I
  #include mu_setup.h
  #include mulib.h
which interrogates the values set in mu_setup to include the right thing.  Maybe too much work and ultimately inflexible but a thought.
4.  We need detailed instructions on the file structure.  Where to install mu lib and where to put my code.  Does my code go into the mulib.c file?  Why is the top-level directory mulib-support?  Shouldn't my code go into the top-level with mulib as the next level?  I'm not sure I want the mulib stuff directly in my directory tree?  I think mulib should be in a /lib directory separate from where my application source goes.  However, maybe in embedded-land things are more combined, I don't know.
5.  Need to rationalize the code a bit.  The mulib.c file you get when you clone git doesn't look like the morse_1.c from the examples.
6.  Need detailed coverage of all APIs - separate from comments in the code.  I hate that.  The git pages should include details of what each developer-visible function does (responsibility, return, errors, exception conditions, etc.); I do not mean that internal functions should be documented in this way - just the things that I would touch if I were porting to a new HW platform.
7.  Similar need for programmers *using* mulib on a platform without needing to port it.  How should my code be structured?  What APIs should I use?  Document all this.
8.  In short, the above few questions talk about a more detailed tutorial.
9.  Why no int main()?  Maybe in embedded-land the runtime is simpler but if no main() then what is the application entry-point?  Explain.
10.  Headers have some confusing code - maybe because its a work-in-progress?  For example disable/enable_interrupts is a while(true) loop.  This seems bad.  Maybe it is supposed to be that way.  If so, code comment should explain.  Also, why would I want to disable interrupts anyway?  Embedded-land should explain this.
11.  Inconsistent use of empty #define.  #define log_enabled 1 but others are just #define someconstant.  Maybe log_enabled has a value because it is also defining a log-level?  Should be consistent.  Also, the comment says "uncomment log_enabled to turn on logging" but it's already uncommented.  Not high-importance because the intent is clear but the code as it comes out of git should be consistent.
12.  Should there be a makefile?  Again, make it very clear how to build an application using mulib, how to port to a new HW platform and how to pick-and-choose components.


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




