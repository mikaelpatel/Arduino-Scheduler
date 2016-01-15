# Arduino-Scheduler

This library implements the Arduino Scheduler class. Multiple loop()
functions may be started and run in a collaborative multi-tasking
style. The threads are run until they call yield().

The Arduino yield() function is replaced by a SchedulerClass
implementation with a context switch. A thread loop() function must
call yield() or delay().

Threads should be viewed as static and continuous. This implementation
does not allocated threads on the heap. They are allocated on the normal
stack and linked into a cyclic run queue.

The Scheduler is a single-ton.



