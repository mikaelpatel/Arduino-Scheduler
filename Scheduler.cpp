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
#include <alloca.h>

Scheduler scheduler;

Scheduler::thread_t Scheduler::s_main;
Scheduler::thread_t* Scheduler::s_running = &Scheduler::s_main;
size_t Scheduler::s_top = Scheduler::DEFAULT_STACK_SIZE;

void Scheduler::begin(size_t stackSize)
{
  s_main.next = &s_main;
  s_top = stackSize;
}

void Scheduler::start(loopFunc loop, size_t stackSize)
{
  void* stack = alloca(s_top);
  s_top += stackSize;
  init(stack, loop);
}

void Scheduler::yield()
{
  if (setjmp(s_running->context)) return;
  s_running = s_running->next;
  longjmp(s_running->context, 1);
}

void Scheduler::init(void* stack, loopFunc loop)
{
  thread_t thread;
  thread.next = s_main.next;
  s_main.next = &thread;
  if (setjmp(thread.context)) while (1) loop();
}


