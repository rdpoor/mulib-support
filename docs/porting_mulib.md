# Porting mulib to a new platform
A key design goal for `mulib` is that porting it to a new platform should be easy.

This document takes you through the steps required to extend mulib to run on the platform of your choice.

## TL;DR

1. Incorporate mulib and mulib-platform into your IDE project
2. Modify mulib-platform/mu_config.h for your platform
3. Modify mulib-platform/mu_time.h for your platform
4. Build and run your project on your platform.

## Details

A typical mulib-based project will incorporate the `mulib` repository
in its entirety within the IDE project itself.  In addition, you will create a directory named `mulib-platform` that will contain the code that interfaces mulib to your platform.  As an example, your project may look something like this:


```
my_project/
  mulib/           // the mulib source files
    core/
    extras/
    mulib.h
  mulib-platform/  // your platform-specific interface
    mu_config.h    // platform-specific definitions used by mulib
    mu_time.c      // platform-specific time implementation
    mu_time.h      // platform-specific time declarations
  src/
```

## Step 1: Incorporate mulib and mulib-platform into yoru IDE project

The details of creating an IDE project vary from IDE to IDE, so this section is abstract out of necessity.  But the basic idea is as follows:

1. Use your IDE to create my-project.
2. % cd my-project; git clone https://github.com/rdpoor/mulib
3. % mkdir mulib-platform; cp mulib/platform/* mulib-platform

Note that in some IDEs, it may be preferable to keep the mulib and mulib-platform directories outside of the my_project directory.  Your mileage may vary.

## Step 2: Modify mulib-platform/mu_config.c for your platform

If you followed the directions above, you will find a generic version of mu_config.h in your mulib-platform directory.  Here's what you need to change:

### Housekeeping

Near the top of the file, comment out (or delete) the line containing:

    #error "Replace mu_config.h with your platform-specific file"

Near the top of the file, comment out (or delete) the line containing:

    #if 0

At the bottom of the file, comment out or delete the line containing:

    #endif // #if 0


### Interrupt management

Replace the two lines that read:


    #define MU_DISABLE_INTERRUPTS() do {} while(0)
    #define MU_ENABLE_INTERRUPTS() do {} while(0)

with definitions that disable and enable global interrupts on your platform.  For example, this might end up reading:

    #define MU_DISABLE_INTERRUPTS() _disable_irq
    #define MU_ENABLE_INTERRUPTS() _enable_irq

### General switches

If you want to enable logging in your system, un-comment the line that reads:

    #define MU_LOG_ENABLED 1

Task Profiling allows you to track the number of times a task was called and to keep track of the amount of time spent running each task.  This comes as the expense of some additional storage ovehead and execution time.  To enable task profiling, uncomment the line that reads:

    #define MU_TASK_PROFILING

Note: `MU_CAN_SLEEP` is not used in the current version and will be deprecated.

    // #define MU_CAN_SLEEP

### Floating point support

If your platform has native support for floating point operations, or if your application favors including floating-point libraries, you might choose to let `mulib` use floating point functions for some of its operations.  To do so, uncomment one of the two lines that read:

    // #define MU_FLOAT float
    // #define MU_FLOAT double

### Supporting time functions

Real-time functions are core to mulib, so you'll need to define a representation of time appropriate for your platform.  Generally speaking, there are two factors to consider when choosing a representation for time:

* **Resolution**: For maximum responsiveness, you usually want a representation of time with sub-millisecond resolution.  In many applications, a real-time clock with a frequency of 32768 Hz is appropriate, and gives a resolution of 30.5 microseconds.
* **Range**: The contract of `mu_sched` is to schedule tasks to be performed at some point in the future.  The maximum limit of how far in the future is determined by the largest time difference that you can represent.  In practice, 32 bits of time (or 31 bits of signed time difference) is acceptable.  A real-time clock of 32768 Hz and a time difference of 31 bits gives you the ability to schedule things 65,536 seconds (18.2 hours) in the future.

`mulib` depends on three type definitions for time which you must specify:

**`mu_time_t`** must be a data type capable of holding time since the processor rebooted, and typically run for a least one day before rolling over.  This is just a guidline, though: your application may have different needs.

**`mu_time_dt`** must be a data type capable of holding the difference between two `mu_time_t` objects.

**`mu_time_ms_dt`** must be a data type capable of holding a time difference in milliseconds.

Note that these type definitions do not need to be integers.  For example, in an 8-bit processor, you might choose to implement mu_time_t as follows:

    typedef struct {
      uint8_t t0;
      uint8_t t1;
      uint8_t t2;
      uint8_t t3;
    } mu_time_t;

See the section on `mu_time.h` to see how you actually use these definitions.

## Step 3: Modify mulib-platform/mu_time.c for your platform

The file `mulib-platform/mu_time.h` provides the declarations and documentation for the functions that you must provide in `mulib-platform/mu_time.c`.

A simple strategy is to copy all the function declarations from mu_time.h into mu_time.c and then convert those declarations into functions as appropriate for your platform.

The most complex platform-specific function you'll need to write is probably `mu_time_now()`, whose contract is to return the value of your real-time clock.

## Step 4: Build and run your project on your platform.

Having made the modifications above, you should be able to compile and run a mulib-based example.

[TBD: provide a very short "blink LED" source code example]
