# blink-suite

`blink-suite` is an introduction to MuLib best practices that blinks an LED to
demonstrate a series of increasingly complex examples.  It starts with the
canonical 'blink an LED' application and progressively builds upon that to make
an application that blinks an entire string in Morse code.

Every embedded system starts with the canonical "blink an LED" program, and this
suite is no different.  But we use that as a starting point for more complex and
different kinds of tasks.

All of the following examples assume you have a "board support package"  (bsp)
that hides the details of controlling LEDs, and assume that you have provided
the function:

```
bsp_set_led(led_id_t led_id, bool on);
```

that takes two arguments: the `led_id` specifies which LED, the `on` argument
turns the LED on if true and turns it off if false.

## `blink_basic`: The singleton task pattern

`blink_basic` introduces the core concepts of `mulib`, using `mu_sched` and
`mu_task` too periodically blink an LED.

## `blink_multi`: The multi-instance task pattern

`blink_multi` is similar to `blink_basic`, but blinks two LEDs.  This shows how
you can support multiple instances of a task with minimum replication of code.

## `blink_letter`: A non-trivial state machine pattern

`blink_letter` demonstrates a a mu_task with a somewhat more complex state
machine.  Once started, the `blink_letter` task will blink an LED using Morse
code to spell out a single letter.

It also shows how to implement an `on_completion` task that gets triggered when
the `blink_letter` task completes.

## `blink_string`: The nested-task pattern

Blinking a single letter in Morse code is okay, but wouldn't it be better if we
could blink out a whole sentence?  `blink_string` introduces nested tasks, where
one task triggers a second task, and the second task notifies the first when it
completes.  

You can use nested tasks to build complex system, even though each task is
relatively simple.

In this case, the `blink_string` task emits one letter at a time and calls the
`blink_letter` task to blink that letter in Morse code.  Each time `blink_letter`
completes, it calls back to the `blink_string` task to request the next letter.
The process stops when `blink_strimg` has no more letters to emit.
