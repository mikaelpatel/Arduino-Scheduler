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
#endif

// Stack magic pattern
const uint8_t MAGIC = 0xa5;

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

bool SchedulerClass::start(func_t taskSetup, func_t taskLoop, size_t stackSize)
{
  // Check called from main task and valid task loop function
  if ((s_running != &s_main) || (taskLoop == NULL)) return (false);

  // Adjust stack size with size of task context
  stackSize += sizeof(task_t);

  // Allocate stack(s) and check if main stack top should be set
  size_t frame = RAMEND - (size_t) &frame;
  uint8_t stack[s_top - frame];
  if (s_main.stack == NULL) {
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

void SchedulerClass::init(func_t setup, func_t loop, const uint8_t* stack)
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
    if (setup != NULL) setup();
    while (1) loop();
  }
}

extern "C"
void yield(void)
{
  Scheduler.yield();
}
