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

class Scheduler {
public:
  static const size_t DEFAULT_STACK_SIZE = 128;
  typedef void (*loopFunc)();
  static void begin(size_t stackSize = DEFAULT_STACK_SIZE);
  static void start(loopFunc loop, size_t stackSize = DEFAULT_STACK_SIZE);
protected:
  static void init(void* stack, loopFunc loop);
  static void yield();
  struct thread_t {
    thread_t* next;
    jmp_buf context;
  };
  static thread_t s_main;
  static thread_t* s_running;
  static size_t s_top;
  friend void ::yield();
};

extern Scheduler scheduler;

void yield()
{
  scheduler.yield();
}

#endif
