# Arduino-Scheduler

This library implements a sub-set of Arduino Scheduler class. Multiple
loop() functions may be started and run in a collaborative
multi-tasking style. The tasks are run until they call yield().

The Arduino yield() function is replaced by a SchedulerClass
implementation which allows context switching. A task must call
yield() or delay().

Tasks should be viewed as static and continuous. This implementation
does not allocated tasks on the heap. They are allocated on the normal
stack and linked into a cyclic run queue. One-shot tasks are not
implemented.

The task start function is extended with a setup function
reference. Tasks are started with Scheduler.start(taskSetup,
taskLoop). The tasks will start execution when the main task
yields. The taskSetup is called first and once by the task followed by
repeated calls to taskLoop. This works just as the Arduino setup and
loop functions.

The Scheduler is a single-ton. The library creates the single
instance.



