# A mildly opinionated style guide to MuLib

MuLib strives to be a framework that lets you do what you want without getting
in your way.  And while MuLib doesn't tell you how to structure your code, there
are a few MuLib-specific programming patterns that have proven to be durable and
useful.  The mulib-patterns suite presents a few of these patterns in the form of
spelled-out examples.

## blink-suite

`blink-suite` is an introduction to MuLib best practices that blinks an LED to
demonstrate a series of increasingly complex examples.  It starts with the
canonical 'blink an LED' application and progressively builds upon that to make
an application that blinks an entire string in Morse code.


## advanced-patterns

`advanced-patterns` demonstrates higher-order patterns for tasks, including how
to wait until a series of other tasks complete (similar to the 'join' function
in multi-threaded systems), wait with timeout, and interrupt handling.
