/**
 * @file Scheduler.cpp
 * @version 1.2
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

#include "Scheduler.h"
#include <Arduino.h>

// Configuration: SRAM and heap handling
#if defined(TEENSYDUINO) && defined(__MK20DX256__)
#undef ARDUINO_ARCH_AVR
#define TEENSY_ARCH_ARM
#define RAMEND 0x20008000

#elif defined(ARDUINO_ARCH_AVR)
extern int __heap_start, *__brkval;
extern char* __malloc_heap_end;
extern size_t __malloc_margin;

#elif defined(ARDUINO_ARCH_SAM)
#define RAMEND 0x20088000

#elif defined(ARDUINO_ARCH_SAMD)
#define RAMEND 0x20008000

#elif defined(ARDUINO_ARCH_ESP8266)
// https://github.com/esp8266/esp8266-wiki/wiki/Memory-Map
#define RAMSTART  0x3FFE8000
#define RAMSIZE   0x18000
#define RAMEND    RAMSTART + RAMSIZE - 1
//#define RAMEND 0x3fffffb0

#endif


// Single-ton
//SchedulerClass Scheduler;
SchedulerClass& sched(void)
{
  static SchedulerClass ans = SchedulerClass();
  return ans;
}

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
size_t SchedulerClass::s_top = SchedulerClass::DEFAULT_STACK_SIZE;

#if defined(ARDUINO_ARCH_ESP8266)
extern "C" {
#include "os_type.h"
//#include "signal.h"
#include "user_interface.h"
}
#include "Scheduler/Semaphore.h"

#define LOOP_TASK_PRIORITY 1
#define LOOP_QUEUE_SIZE    1

//void handler (int signum) { printf("SIGNAL\n"); };

extern "C" uint32_t system_get_time();
extern "C" void preloop_update_frequency();

static uint32_t g_micros_at_task_start = 0;

extern "C" void esp_yield(void){ yield(); }

extern "C" void esp_scheduler(void){}

extern "C" void optimistic_yield(uint32_t interval_us){ yield(); }

extern "C" void delay(unsigned long interval_ms)
{
  uint32_t g_micros_at_task_start = system_get_time();

  while((system_get_time() - g_micros_at_task_start) < interval_ms*1000)
  {
      system_soft_wdt_feed();
      yield();
  }
}

extern "C" void delay_until(uint32_t clock)
{
    uint32_t current_clock;

    while((current_clock = system_get_time()) < clock)
    {
        system_soft_wdt_feed();
        yield();
    }

    if((current_clock - clock) > 100)
    {
#ifdef DEBUG_SCHEDULER
        printf_P(PSTR("deadline miss\n"));
#endif
    }
}

extern "C" unsigned int get_clock()
{
    return system_get_time();
}

extern "C" void loop_task(os_event_t *events)
{
  static bool one = true;

  g_micros_at_task_start = system_get_time();

  //signal (SIGSEGV, handler);

  if(one)
  {
    one = false;

    preloop_update_frequency();
    Scheduler.begin(0x4000);
    setup();

    if(!Scheduler.start(NULL, loop, 0x1000))
    {
      panic();
    }
  }

  yield();

  ets_post(LOOP_TASK_PRIORITY, 0, 0);
}

static Semaphore spiffs(1);

extern "C" void esp_spiffs_lock(uint32_t* fs)
{
    spiffs.wait(1);
}

extern "C" void esp_spiffs_unlock(uint32_t* fs)
{
    spiffs.signal(1);
}

#endif


bool SchedulerClass::begin(size_t stackSize)
{
  static bool initiated = true;

  if(initiated)
  {
    initiated = false;
    // Set main task stack size
    s_top = stackSize;
  }

#if defined(ARDUINO_ARCH_ESP8266)
  // fill the remaining stack with a pattern
  uint8_t * fr = (uint8_t*)RAMEND-STACK_MAX;
  memset(fr, 0x08, STACK_MAX-stackSize);
#endif

  return true;
}

bool SchedulerClass::start(func_t taskSetup, func_t taskLoop, size_t stackSize)
{
  // Check called from main task and valid task loop function
  if ((s_running != &s_main) || (taskLoop == NULL)) return (false);

  // Adjust stack size with size of task context
  stackSize += sizeof(task_t);

  // Allocate stack(s) and check if main stack top should be set
  size_t frame = RAMEND - (size_t) &frame;

#ifdef DEBUG_SCHEDULER
  printf_P(PSTR("%p\n"), (size_t) &frame);
  printf_P(PSTR("s_top - frame=%u\n"), s_top - frame);
#endif

  uint8_t stack[s_top - frame];
  if (s_main.stack == NULL) s_main.stack = stack;

#if defined(ARDUINO_ARCH_AVR)
  // Check that the task can be allocated
  int HEAPEND = (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
  int STACKSTART = ((int) stack) - stackSize;
  HEAPEND += __malloc_margin;
  if (STACKSTART < HEAPEND) return (false);

  // Adjust heap limit
  __malloc_heap_end = (char*) STACKSTART;
#endif

#if defined(ARDUINO_ARCH_SAM)  || \
    defined(ARDUINO_ARCH_SAMD) || \
    defined(TEENSY_ARCH_ARM)   || \
    defined(ARDUINO_ARCH_ESP8266)
  // Check that the task can be allocated
  if (s_top + stackSize > STACK_MAX) return (false);
#endif

  // Adjust stack top for next task allocation
  s_top += stackSize;

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

extern "C" void yield(void)
{
  Scheduler.yield();
}
