/**
 * @file SchedulerTaskCompletionQueue.ino
 * @version 1.0
 * @author Andrei Stoica
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
 *
 * @section Description
 * This Arduino sketch shows how to use the Scheduler library;
 * Worker Design Pattern; Functions (tasks) are queued and executed by
 * work threads.
 */

#include <Scheduler.h>
#include <Scheduler/Queue.h>

// Check for SparkFun SAMD21 Breakout
#if defined(ARDUINO_ARCH_SAMD) && (USB_PID == 0x8D21)
#define Serial SerialUSB
#endif

// Queue of tasks (function pointers)
typedef SchedulerClass::func_t task_t;
const unsigned int TASKQ_MAX = 8;
Queue<task_t, TASKQ_MAX> taskq;

// Example task: Calculate sum(i): i=0..100
volatile unsigned int sum100;

void sumTo100()
{
  const unsigned int N = 100;
  sum100 = 0;
  for (unsigned int i = 0; i <= N; i++) {
    sum100 += i;
    delay(random(200));
  }
}

// Example task2: Calculate fac(10)
volatile unsigned long int fact10;

void factTo10()
{
  const unsigned int N = 10;
  fact10 = 1;
  for (unsigned int i = 1; i <= N; i++) {
    fact10 *= i;
    delay(random(2000));
  }
}

// Example task3: simple background task (push back example)
void printAlive()
{
  Serial.print(millis());
  Serial.println(F(":printAlive::alive..."));
  Serial.flush();
  delay(1000);

  task_t task = printAlive;
  taskq.push(&task);
}

void setup()
{
  Serial.begin(57600);
  while (!Serial);
  Serial.println(F("SchedulerTaskCompletionQueue: started"));
  Serial.flush();

  // Start two worker
  Scheduler.startLoop(worker);
  Scheduler.startLoop(worker);

  // Push tasks
  task_t task;
  task = sumTo100;
  Serial.print(millis());
  Serial.print(F(":setup::push task @sumTo100 = "));
  Serial.println(int(&sumTo100));
  taskq.push(&task);

  task = factTo10;
  Serial.print(millis());
  Serial.print(F(":setup::push task @factTo10 = "));
  Serial.println(int(&factTo10));
  taskq.push(&task);

  task = printAlive;
  Serial.print(millis());
  Serial.print(F(":setup::push task @printAlive = "));
  Serial.println(int(&printAlive));
  taskq.push(&task);

  // Note: The printAlive task will start when either sumTo100 or
  // factTo10 are completed as there are only two workers.
}

void loop()
{
  Serial.print(millis());
  Serial.print(F(":loop::sum100 = "));
  Serial.print(sum100);
  Serial.print(F(", fact10 = "));
  Serial.println(fact10);
  delay(1000);
}

void worker()
{
  task_t task;
  taskq.pull(&task);
  task();
}
