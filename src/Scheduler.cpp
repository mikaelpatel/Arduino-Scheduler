/**
 * @file Scheduler.cpp
 * @version 1.5
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

#include "Scheduler.h"
#include <Arduino.h>

// Single-ton
SchedulerClass Scheduler;

// Main task and run queue
SchedulerClass::task_t SchedulerClass::s_main = {
  &SchedulerClass::s_main,
  &SchedulerClass::s_main,
  { 0 },
  NULL
};

// Reference running task
SchedulerClass::task_t* SchedulerClass::s_running = &SchedulerClass::s_main;

// Initial top stack for task allocation
size_t SchedulerClass::s_top = SchedulerClass::DEFAULT_MAIN_STACK_SIZE;

bool SchedulerClass::begin(size_t stackSize)
{
  // Set main task stack size
  s_top = stackSize;
  return (true);
}

void SchedulerClass::yield()
{
  // Caller will continue here on yield
  if (setjmp(s_running->context)) return;

  // Next task in run queue will continue
  s_running = s_running->next;
  longjmp(s_running->context, true);
}

size_t SchedulerClass::stack()
{
  const uint8_t* sp = s_running->stack;
  size_t bytes = 0;
  while (*sp++ == MAGIC) bytes++;
  return (bytes);
}

extern "C"
void yield(void)
{
  Scheduler.yield();
}
