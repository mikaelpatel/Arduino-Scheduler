/**
 * @file Scheduler.cpp
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

#include "Scheduler.h"

#define UNUSED(x) (void) (x)

SchedulerClass Scheduler;

SchedulerClass::thread_t SchedulerClass::s_main = {
  &SchedulerClass::s_main,
  { 0 }
};

SchedulerClass::thread_t* SchedulerClass::s_running = &SchedulerClass::s_main;

size_t SchedulerClass::s_top = SchedulerClass::DEFAULT_STACK_SIZE;

bool SchedulerClass::begin(size_t stackSize)
{
  s_top = stackSize;
  return (true);
}

bool SchedulerClass::start(loopFunc loop, size_t stackSize)
{
  char stack[s_top];
  extern int __heap_start, *__brkval;
  extern char* __malloc_heap_end;
  extern size_t __malloc_margin;
  int HEAPEND = (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
  int STACKSTART = ((int) stack) - stackSize;
  HEAPEND += __malloc_margin;
  if (STACKSTART < HEAPEND) return (false);
  s_top += stackSize;
  __malloc_heap_end = (char*) STACKSTART;
  init(stack, loop);
  return (true);
}

void SchedulerClass::yield()
{
  if (setjmp(s_running->context)) return;
  s_running = s_running->next;
  longjmp(s_running->context, 1);
}

void SchedulerClass::init(void* stack, loopFunc loop)
{
  UNUSED(stack);
  thread_t thread;
  thread.next = s_main.next;
  s_main.next = &thread;
  if (setjmp(thread.context)) while (1) loop();
}

extern "C"
void yield(void)
{
  Scheduler.yield();
}

