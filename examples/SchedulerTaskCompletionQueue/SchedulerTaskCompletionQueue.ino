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
 * Queue events between tasks.
 */

#include <Scheduler.h>
#include <Scheduler/Queue.h>

// Check for SparkFun SAMD21 Breakout
#if defined(ARDUINO_ARCH_SAMD) && (USB_PID == 0x8D21)
#define Serial SerialUSB
#endif

Queue<SchedulerClass::func_t, 4> taskq;

volatile unsigned int sum100;

volatile unsigned int fact10;

void sumTo100()
{
  const unsigned int N = 100;
  sum100 = 0;
  for (unsigned int i = 0; i != N+1; ++i) 
  {
	  sum100 += i;
	  delay(random(10));
  }
}

void factTo10()
{
  const unsigned int N = 10;
  fact10 = 1;
  for (unsigned int i = 1; i != N+1 ; ++i) 
  {
	  fact10 *= i;
	  delay(random(10));
  }
}

void printAlive()
{
  Serial.print(millis());
  Serial.print(F(":printAlive::"));
  Serial.println(F("alive..."));
  yield();
}

void printAliveLoop()
{
  Serial.print(millis());
  Serial.print(F(":printLoop::"));
  Serial.println(F("alive loop..."));
  delay(500);
}

void setup()
{
  Serial.begin(57600);
  while (!Serial);
  Serial.println(F("SchedulerTaskCompletionQueue: started"));
  Serial.flush();

  SchedulerClass::func_t task;

  task = sumTo100;
  Serial.print(millis());
  Serial.print(F(":setup::push task sumto100="));
  Serial.println(int(&sumTo100));
  taskq.push(&task);
  
  task = factTo10;
  Serial.print(millis());
  Serial.print(F(":setup::push task factTo10="));
  Serial.println(int(&factTo10));
  taskq.push(&task);
  
  task = printAlive;
  Serial.print(millis());
  Serial.print(F(":setup::push task printAlive="));
  Serial.println(int(&printAlive));
  taskq.push(&task);

  // Start two event handlers
  Scheduler.startLoop(taskRunner);
  Scheduler.startLoop(taskRunner);
  Scheduler.startLoop(printAliveLoop);
}

void loop()
{
  delay(1000);
  
  Serial.print(millis());
  Serial.print(F(":loop sum100="));
  Serial.println(sum100);
  
  Serial.print(millis());
  Serial.print(F(":loop fact10="));
  Serial.println(fact10);
}

void taskRunner()
{
  // Pull events
  SchedulerClass::func_t* task;
  taskq.pull(task);
  (*task)();
}
