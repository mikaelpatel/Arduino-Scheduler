# Arduino-Scheduler

This library implements the Arduino Scheduler class. Multiple loop()
functions may started and run in a collaborative multi-tasking
style. The threads are run until they call yield(). They are not
interrupted.

The Arduino yield() function is replaced by a SchedulerClass
implementation with a context switch. A thread must call yield() or
delay().

The Scheduler is a single-ton.



