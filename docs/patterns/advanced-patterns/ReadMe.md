## `join-task`: The join pattern

Sometimes one task will fire a number of nested tasks, but then must to wait
until all the nested tasks have completed.  This is conceptually similar to the
`join` function found in multi-threaded environments, but implemented somewhat
differently.

In this example, the `join-task` task fires off multiple nested tasks and then
uses the `join` module to wait until all the sub-tasks have completed.

## `join-task-timeout`: The join-with-timeout pattern

But what if one of the sub-tasks never completes?  You might want your main task
to stop waiting after an appropriate amount of time has elapsed.  `mulib` makes
it easy to add a timeout feature to the join module.

## `task-interrupt`: The interrupt handling pattern

mulib provides a simple and efficient mechanism for handling interrupts.  In
brief, you simply call `mu_sched_isr_task_now()` at interrupt level, and the
task that you schedule from interrupt level will be the next task executed in
the foreground.

## `blink_concerns`: The Separation of Concerns pattern
