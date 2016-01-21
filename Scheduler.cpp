/**
 * @file Scheduler.cpp
 * @version 1.0
 *
 * @section License
 * Copyright (C) Mikael Patel, 2015-2016
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

#define UNUSED(x) (void) (x)

extern int __heap_start, *__brkval;
extern char* __malloc_heap_end;
extern size_t __malloc_margin;

SchedulerClass Scheduler;

SchedulerClass::task_t SchedulerClass::s_main = {
  &SchedulerClass::s_main,
  &SchedulerClass::s_main,
  { 0 },
  NULL
};

SchedulerClass::task_t* SchedulerClass::s_running = &SchedulerClass::s_main;

size_t SchedulerClass::s_top = SchedulerClass::DEFAULT_STACK_SIZE;

bool SchedulerClass::begin(size_t stackSize)
{
  // Set main task stack size
  s_top = stackSize;
  return (true);
}

bool SchedulerClass::start(func_t taskSetup, func_t taskLoop, size_t stackSize)
{
  // Check called from main task and valid task loop function
  if ((s_running != &s_main) || (taskLoop == NULL)) return (false);

  // Adjust stack size with size of task context
  stackSize += sizeof(task_t);

  // Check that the task can be allocated
  uint8_t stack[s_top];
  if (s_main.stack == NULL) s_main.stack = stack;
  int HEAPEND = (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
  int STACKSTART = ((int) stack) - stackSize;
  HEAPEND += __malloc_margin;
  if (STACKSTART < HEAPEND) return (false);

  // Adjust stack top for next task allocation
  s_top += stackSize;

  // Adjust heap limit
  __malloc_heap_end = (char*) STACKSTART;

  // Initiate task with given functions and stack top
  init(taskSetup, taskLoop, stack - stackSize);
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
  unsigned char marker;
  return (&marker - s_running->stack);
}

void SchedulerClass::init(func_t setup, func_t loop, const uint8_t* stack)
{
  UNUSED(stack);
  task_t task;

  // Add task last in run queue
  task.next = &s_main;
  task.prev = s_main.prev;
  s_main.prev->next = &task;
  s_main.prev = &task;
  task.stack = stack;

  // Create context for new task, caller will return
  if (setjmp(task.context)) {
    if (setup != NULL) setup();
    while (1) loop();
  }
}

extern "C"
void yield(void)
{
  Scheduler.yield();
}

