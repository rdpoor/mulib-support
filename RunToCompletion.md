# What is a Run To Completion scheduler?

A Run To Completion (RTC) scheduler is a software resource that defers execution of a function until some time in the future.  As the name suggests, once a function is called by the RTC scheduler, the function must return in order for the scheduler to process the next runnable function.  (But see "What about Interrupts?" below.)  This is in direct contrast to a Preemptive scheduler, which is able to suspend execution of a function at any time in order to process other, higher-priority functions.

## What are the advantages of RTC schedulers?

For embedded microcontrollers and other resource-constrained processing environments, RTC schedulers offer several advantages over their Preemptive cousins:

* **Highly Portable:** Since scheduling is done via ordinary function calls, the RTC  scheduler does not need custom assembly code to save and restore stack state. This means that the same code will work, independent of the target hardware, compiler or IDE used.

* **Very fast context switching:** Since switching from one scheduled function to another is just a pair of function calls, it's not necessary to save and restore the entire processor state.

* **Thread safe:** This is a key selling point for RTC schedulers.  Since there's really only one thread, external library functions and I/O calls don't have to be made thread-safe.  There's no need to wrap external functions with mutex / semaphore protection, and many subtle to trace errors are avoided.

* **Small RAM footprint:** Most Preemptive schedulers require you to pre-allocate the maximum predicted stack space for each task, resulting in a lot of wasted space.  By contrast, since an RTC scheduler only has one execution stack the full stack is available at all times for each running function.

* **Small code footprint:** A typical RTC scheduler can be written in one page of C code with a remarkably small footprint.

* **Simplified coding and debugging:** Another consequence of the single-threaded nature of the RTC scheduler is that you no longer worry about inter-thread locking mechanisms, such as mutexes and semaphores. And since there is only one thread, there are fewer state variables to examine while debugging.

## What are the disadvantages to RTC schedulers?

Despite all of the advantages listed above, a Run-To-Completion scheduler is not the ultimate panacea for all applications.  Compared to a Preemptive scheduler, an RTC scheduler requires some changes in design philosophy and mindset:

* **RTCs require non-blocking functions:** The fundamental rule of any function called by a RTC scheduler is that it must never block.  This requires a change in mind-set when writing the code -- for example you want to call is_key_pressed() before conditionally calling getchar() -- but it's proven not to be a difficult change.

* **The slowest function determines the maximum latency of the RTC:** Since each function must run to completion, the slowest function sets the upper bound on the RTC's responsiveness.  If a function does require a lot of time, it can usually be split into smaller, faster functions and/or partitioned with appropriate state variables.

## Neither Here Nor There:

There are several traits that RTC schedulers share with Preemptive schedulers that are worth mentioning:

* **Static Allocation:** Like a Preemptive system, the storage required for an RTC may be pre-allocated statically.

* **Priority Scheduling:** An RTC scheduler may implement task priorities, but since all functions return quickly, priorities are not as important as with Preemptive schedulers.  (But see "What about Interrupts?" below).

* **Flexible Clock Source:** A well designed RTC scheduler is clock-agnostic: it doesn't care where it gets its time base from.  It's worth noting that the choice of "tick frequency" is less important in an RTC than in a Preemptive scheduler, since timestamps are used only to determine the order in which scheduled functions are performed.

* **Plays Nice With Sleep Modes:** Like a Preemptive scheduler, an RTC scheduler's idle task can simply put the processor to sleep until the next scheduled function when there are no more runnable functions.

## What about Interrupts?

Almost every embedded system has some asynchronous source of events, usually implemented as interrupts.  In mulib, interrupts are processed as follows:

At interrupt level, the ISR queues a task at the end of an interrupt-safe "isr queue" and dismisses the interrupt quickly. At foreground level, when the RTC scheduler completes the current function, it sequentially moves each task in the isr queue into the main schedule until the isr queue is empty. The mulib scheduler then proceeds with regularly scheduled tasks.

## Implementation

Over time, I've waffled between two basic implementations of RTC schedulers: array based and linked-list based.  Both of them maintain time ordered tasks of {timestamp, function pointer, user-supplied argument}.

The array based version keeps the "furthest in the future" task at element [0] and does memcpy() operations to insert new tasks in the array.  This approach means that removing the "soonest" task is as easy as moving an array index.  And since tasks in the near future are more common than tasks in the far future, this approach minimizes the amount of memory that must be memcpy'd when inserting a new task.

The linked-list version keeps the "soonest" task at the list head and the "furthest at the future" at the far end.  New tasks are allocated from a freelist ("list pop"), the list is traversed to find the correct insertion point.  Processed tasks are popped from the head of the list and pushed back on the freelist.

When inserting a new task, the array-based approach with its O(log N) time is appropriate when there are lots of tasks.  But in practice, a linear search over a linked list -- even though it is O(N) -- has such low overhead that it's faster for most applications.

## In Closing

There are applications in the world that benefit from preemptive schedulers such as used by FreeRTOS.  But in many embedded applications a full preemptive system isn't needed: the added complexity, code overhead and ram usage don't justify the benefits.  In such cases, a carefully designed and fully tested Run-To-Completion scheduler is faster and smaller.
