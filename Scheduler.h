/**
 * @file Scheduler.h
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2015-2016, Mikael Patel
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

class SchedulerClass {
public:
  /**
   * Function prototype (task setup and loop functions).
   */
  typedef void (*func_t)();

  /**
   * Initiate scheduler and main task with given stack size. Should
   * be called before start of any tasks if the main task requires a
   * stack size other than the default size. Returns true if
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
   * omitted (NULL). Returns true if successful otherwise false.
   * @param[in] taskSetup function (may be NULL).
   * @param[in] taskLoop function (may not be NULL).
   * @param[in] stackSize in bytes.
   * @return bool.
   */
  static bool start(func_t taskSetup,
		    func_t taskLoop,
		    size_t stackSize = DEFAULT_STACK_SIZE);

  /**
   * Context switch to next task in run queue.
   */
  static void yield();

  /**
   * Return current task stack size.
   * @return bytes
   */
  static size_t stack();

protected:
  /**
   * Initiate a task with the given functions and stack. When control
   * is yield to the task the setup function is first called and then
   * the loop function is repeatedly called.
   * @param[in] setup task function (may be NULL).
   * @param[in] loop task function (may not be NULL).
   * @param[in] stack top reference.
   */
  static void init(func_t setup, func_t loop, const uint8_t* stack);

  /**
   * Task run-time structure.
   */
  struct task_t {
    task_t* next;		//!< Next task.
    task_t* prev;		//!< Previous task.
    jmp_buf context;		//!< Task context.
    const uint8_t* stack;	//!< Task stack.
  };

#if defined(TEENSYDUINO) && defined(__MK20DX256__)
  /** Default stack size and stack max. */
  static const size_t DEFAULT_STACK_SIZE = 512;
  static const size_t STACK_MAX = 16384;

#elif defined(ARDUINO_ARCH_AVR)
  /** Default stack size. */
  static const size_t DEFAULT_STACK_SIZE = 128;

#elif defined(ARDUINO_ARCH_SAM)
  /** Default stack size and stack max. */
  static const size_t DEFAULT_STACK_SIZE = 512;
  static const size_t STACK_MAX = 32768;

#elif defined(ARDUINO_ARCH_SAMD)
  /** Default stack size and stack max. */
  static const size_t DEFAULT_STACK_SIZE = 512;
  static const size_t STACK_MAX = 16384;

#else
#error "Scheduler.h: board not supported"
#endif

  /** Main task. */
  static task_t s_main;

  /** Running task. */
  static task_t* s_running;

  /** Task stack allocation top. */
  static size_t s_top;
};

/** Scheduler single-ton. */
extern SchedulerClass Scheduler;

/**
 * Syntactic sugar for scheduler based busy-wait for condition;
 * yield until condition is valid. May require volatile condition
 * variable(s).
 * @param[in] cond condition to await.
 */
#define await(cond) while (!(cond)) yield()

#endif
