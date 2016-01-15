/**
 * @file Scheduler.h
 * @version 1.0
 *
 * @section License
 * Copyright (C) Mikael Patel, 2015
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

class SchedulerClass {
public:
  /** Default stack size. */
  static const size_t DEFAULT_STACK_SIZE = 128;

  /**
   * Loop function prototype.
   */
  typedef void (*loopFunc)();

  /**
   * Initiate the scheduler and main thread with given stack size.
   * @param[in] stackSize size in bytes.
   */
  static void begin(size_t stackSize = DEFAULT_STACK_SIZE);

  /**
   * Start the given loop function as a thread with given stack size.
   * @param[in] loop thread function.
   * @param[in] stackSize size in bytes.
   */
  static void start(loopFunc loop, size_t stackSize = DEFAULT_STACK_SIZE);

protected:
  /**
   * Initiate the given loop function as a thread with the given stack.
   * @param[in] stack top reference.
   * @param[in] loop thread function.
   */
  static void init(void* stack, loopFunc loop);

  /**
   * Context switch to next thread.
   */
  static void yield();

  /**
   * Thread run-time structure.
   */
  struct thread_t {
    thread_t* next;		//!< Next thread.
    jmp_buf context;		//!< Thread context.
  };

  /** Main thread. */
  static thread_t s_main;

  /** Running thread. */
  static thread_t* s_running;

  /** Thread stack allocation top. */
  static size_t s_top;

  friend void ::yield();
};

extern SchedulerClass Scheduler;

/**
 * Context switch to next thread. Replace core implementation.
 * Note: Scheduler header may only be included in sketch to avoid
 * multiple definitions of yield.
 */
void yield()
{
  Scheduler.yield();
}

#endif