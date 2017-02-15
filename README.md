# Arduino-Scheduler

This library implements an extended sub-set of the Arduino Scheduler
class. Multiple loop() functions, tasks, may be started and run in a
collaborative multi-tasking style. The tasks are run until they call
yield() or delay(). The Arduino yield() function is replaced by an
implementation in the library that allows context switching.

Tasks should be viewed as static and continuous. This implementation
does not allocate tasks on the heap. They are allocated on the normal
stack and linked into a cyclic run queue. One-shot tasks are not
supported. Instead the task start function is extended with a setup
function reference. Tasks are started with:

````
Scheduler.start(taskSetup, taskLoop [,taskStackSize]).
````
The tasks will start execution when the main task yields. The
_taskSetup_ is called first and once by the task followed by repeated
calls to _taskLoop_. This works just as the Arduino setup() and loop()
functions. There is also an optional parameter, _taskStackSize_. The
default value depends on the architecture (128 bytes for AVR and 1024
bytes for SAM/SAMD/Teensy 3.X).

````
Scheduler.startLoop(taskLoop [,taskStackSize]).
````
A short hand version of start() is available when the taskSetup is NULL.

The Scheduler is a single-ton and the library creates the single
instance.

![screenshot](https://dl.dropboxusercontent.com/u/993383/Cosa/screenshots/Screenshot%20from%202016-01-29%2015%3A24%3A17.png)

This library also includes support for task synchronization and
communication; Semaphores, Queues and Channels. There is also
built-in support for checking the amount of stack head room,
i.e. untouched stack space.

````
int bytes = Scheduler.stack();
````
The Scheduler member function stack() will scan the running task stack
and determine the minimum amount of remaining stack during the
execution so far. The example sketch SchedulerBlinkMax shows how this
can be used.

## Install

Download and unzip the Arduino-Scheduler library into your sketchbook
libraries directory. Rename from Arduino-Scheduler-master to Arduino-Scheduler.

The Scheduler library and examples should be found in the Arduino IDE
File>Examples menu.

## Performance

There are several benchmark sketches in the examples directory. Below
are some of the results.

### Context Switch

Board | us | cycles
------|----|-------
Arduino Mega 2560 (16 MHz) | 12.64 | 203
Arduino Uno, Nano, Pro-Mini, etc (16 MHz) | 11.00 | 176
Sparkfun SAMD21 (48 MHz) | 2.60 | 125
Arduino Due (84 MHz) | 1.36 | 115
Teensy 3.1 (72 MHz) | 1.10 | 80
Teensy 3.6 (180 MHz) | 0.43 | 78
