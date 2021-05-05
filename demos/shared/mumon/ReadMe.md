# mumon - Create and monitor mulib tasks

```
t  Create a task.  Each task runs 10 times.
R  Reset (clear) tasks

 S Name            RunAt Calls MxLatency
+-+----------+----------+-----+---------+
 A MuMon            4434   132       108
 R Heartbeat        4430    15        43
 I Composer                 15        87
 P Printer                1022       114
 I SerialIn                  2        12
 I Task01                    5        74
 I Task02                    6        33
```

## Tasks

### MuMon

Toplevel task.  Triggered once a second by Heartbeat.  Checks for serial input,
calling SerialIn if data is available.  Then calls Composer to print state of
the schedule.

### Heartbeat

Runs once a second and calls MuMon

### Composer

Runs a state machine to compose each printed line, one at a time.  Calls
Printer to emit the line.  Stops after all lines have been printed.

### Printer

Initialized with a pointer to a string, then prints it.  Calls on_completion
when the string has been printed.

### TaskNN

Schedules itself to run at a randomized time in the future.  Repeats 10 times
and then stops.
