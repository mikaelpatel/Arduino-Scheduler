/**
 * @file Scheduler.h
 * @version 1.6
 *
 * @section License
 * Copyright (C) 2015-2017, Mikael Patel
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 */

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <setjmp.h>
#include <stddef.h>
#include <stdint.h>


// Configuration: SRAM and heap handling
#if defined(TEENSYDUINO)
#undef ARDUINO_ARCH_AVR
#define TEENSY_ARCH_ARM
#if defined(__MK20DX256__)
#define RAMEND 0x20008000
#elif defined(__MK64FX512__)
#define RAMEND 0x20020000
#elif defined(__MK66FX1M0__)
#define RAMEND 0x20030000
#endif

#elif defined(ARDUINO_ARCH_AVR)
extern int __heap_start, *__brkval;
extern char* __malloc_heap_end;
extern size_t __malloc_margin;

#elif defined(ARDUINO_ARCH_SAM)
#if !defined(RAMEND)
#define RAMEND 0x20088000
#endif

#elif defined(ARDUINO_ARCH_SAMD)
#if !defined(RAMEND)
#define RAMEND 0x20008000
#endif

#elif defined(ARDUINO_ARCH_STM32F1)
#if !defined(RAMEND)
#define RAMEND 0x20005000
#endif

#endif


class SchedulerClass {
public:

  /// Stack magic pattern
  static constexpr uint8_t MAGIC = 0xa5;

  /**
   * Initiate scheduler and main task with given stack size. Should
   * be called before start of any tasks if the main task requires a
   * stack size other than the default main size. Returns true if
   * successful otherwise false.
   * @param[in] stackSize in bytes.
   * @return bool.
   */
  static bool begin(size_t stackSize);

  /**
   * Start a task with given functions and stack size. Should be
   * called from main task (in setup). The functions are executed by
   * the task. The taskSetup function (if provided) is run once.
   * The taskLoop function is repeatedly called. The taskSetup may be
   * omitted (nullptr). Returns true if successful otherwise false.
   * @param[in] taskSetup function (may be nullptr).
   * @param[in] taskLoop function (may NOT be nullptr).
   * @param[in] stackSize in bytes.
   * @return bool.
   */
  template<typename SetupF, typename LoopF>
  static bool start(SetupF taskSetup, LoopF taskLoop,
		    size_t stackSize = DEFAULT_STACK_SIZE);

  /**
   * Start a task with given taskLoop function and stack size. Should
   * be called from main task (in setup). The taskLoop function is
   * repeatedly called. Returns true if successful otherwise false.
   * @param[in] taskLoop function.
   * @param[in] stackSize in bytes.
   * @return bool.
   */
  template<typename LoopF>
  static bool startLoop(LoopF taskLoop,
			size_t stackSize = DEFAULT_STACK_SIZE)
  {
    return (start(noop, taskLoop, stackSize));
  }

  /**
   * Context switch to next task in run queue.
   */
  static void yield();

  /**
   * Return minimum remaining stack in bytes for running task.
   * The value depends on executed function call depth and interrupt
   * service routines during the execution (so far).
   * @return bytes
   */
  static size_t stack();

private:
  /**
   * Initiate a task with the given functions and stack. When control
   * is yield to the task the setup function is first called and then
   * the loop function is repeatedly called.
   * @param[in] setup task function (may be nullptr).
   * @param[in] loop task function (may not be nullptr).
   * @param[in] stack top reference.
   */
  template<typename SetupF, typename LoopF>
  static void init(SetupF setup, LoopF loop, const uint8_t* stack);

  /**
   * Task run-time structure.
   */
  struct task_t {
    task_t* next;		//!< Next task.
    task_t* prev;		//!< Previous task.
    jmp_buf context;		//!< Task context.
    const uint8_t* stack;	//!< Task stack top.
  };

#if defined(TEENSYDUINO)
  /** Default stack size and stack max. */
  static const size_t DEFAULT_STACK_SIZE = 1024;
  static const size_t DEFAULT_MAIN_STACK_SIZE = 2048;
#if defined(__MK20DX256__)
  /** Teensy 3.2: 64 Kbyte. */
  static const size_t STACK_MAX = 16384;
#elif defined(__MK64FX512__)
  /** Teensy 3.5: 192 Kbyte. */
  static const size_t STACK_MAX = 49152;
#elif defined(__MK66FX1M0__)
  /** Teensy 3.6: 256 Kbyte. */
  static const size_t STACK_MAX = 65536;
#else
#error "Scheduler.h: Teensy board not supported"
#endif

#elif defined(ARDUINO_ARCH_AVR)
  /** Default stack size. Stack max dynamically checked against heap end. */
  static const size_t DEFAULT_STACK_SIZE = 128;
#if defined(USBCON)
  static const size_t DEFAULT_MAIN_STACK_SIZE = 320;
#else
  static const size_t DEFAULT_MAIN_STACK_SIZE = 256;
#endif

#elif defined(ARDUINO_ARCH_SAM)
  /** Default stack size and stack max. */
  static const size_t DEFAULT_STACK_SIZE = 1024;
  static const size_t DEFAULT_MAIN_STACK_SIZE = 2048;
  static const size_t STACK_MAX = 32768;

#elif defined(ARDUINO_ARCH_SAMD)
  /** Default stack size and stack max. */
  static const size_t DEFAULT_STACK_SIZE = 1024;
  static const size_t DEFAULT_MAIN_STACK_SIZE = 2048;
  static const size_t STACK_MAX = 16384;

#elif defined(ARDUINO_ARCH_STM32F1)
  /** Default stack size and stack max. */
  static const size_t DEFAULT_STACK_SIZE = 1024;
  static const size_t DEFAULT_MAIN_STACK_SIZE = 2048;
  static const size_t STACK_MAX = 16384;

#else
#error "Scheduler.h: Arduino board not supported"
#endif

  /** Main task. */
  static task_t s_main;

  /** Running task. */
  static task_t* s_running;

  /** Task stack allocation top. */
  static size_t s_top;

  /** No-op method to be used when no Setup provided */
  static void noop() {};
};


template<typename SetupF, typename LoopF>
bool SchedulerClass::start(SetupF taskSetup, LoopF taskLoop, size_t stackSize)
{
  // Check called from main task
  if (s_running != &s_main) return (false);

  // Adjust stack size with size of task context
  stackSize += sizeof(task_t);

  // Allocate stack(s) and check if main stack top should be set
  size_t frame = RAMEND - (size_t) &frame;
  uint8_t stack[s_top - frame];
  if (s_main.stack == nullptr) {
    s_main.stack = stack;
    memset(stack, MAGIC, s_top - frame);
  }

#if defined(ARDUINO_ARCH_AVR)
  // Check that the task can be allocated
  int HEAPEND = (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
  int STACKSTART = ((int) stack) - stackSize;
  HEAPEND += __malloc_margin;
  if (STACKSTART < HEAPEND) return (false);

  // Adjust heap limit
  __malloc_heap_end = (char*) STACKSTART;
#else
  // Check that the task can be allocated
  if (s_top + stackSize > STACK_MAX) return (false);
#endif

  // Adjust stack top for next task allocation
  s_top += stackSize;

  // Fill stack with magic pattern to allow detect of stack usage
  memset(stack - stackSize, MAGIC, stackSize - sizeof(task_t));

  // Initiate task with given functions and stack top
  init(taskSetup, taskLoop, stack - stackSize);
  return (true);
}


template<typename SetupF, typename LoopF>
void SchedulerClass::init(SetupF setup, LoopF loop, const uint8_t* stack)
{
  // Add task last in run queue (main task)
  task_t task;
  task.next = &s_main;
  task.prev = s_main.prev;
  s_main.prev->next = &task;
  s_main.prev = &task;
  task.stack = stack;

  // Create context for new task, caller will return
  if (setjmp(task.context)) {
    if (setup != nullptr) setup();
    while (1) loop();
  }
}

/** Scheduler single-ton. */
extern SchedulerClass Scheduler;

/**
 * Syntactic sugar for scheduler based busy-wait for condition;
 * yield until condition is valid. May require volatile condition
 * variable(s).
 * @param[in] cond condition to await.
 */
#define await(cond) while (!(cond)) Scheduler.yield()

#endif
