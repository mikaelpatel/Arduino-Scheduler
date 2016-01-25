/**
 * @file SchedulerBenchmarkQueue.ino
 * @version 1.0
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
 *
 * @section Description
 * Benchmark Scheduler Queue.
 *
 * @section Results Arduino 1.6.7
 * Events per second (push/pull)
 * Board\QUEUE_MAX	2	8
 * Arduino Pro-Mini	24017	92589
 * Arduino Mega 2560	21455	86877
 * Arduino Due		178128	670334
 */

#include <Scheduler.h>
#include <Scheduler/Queue.h>

const unsigned int QUEUE_MAX = 2;
typedef int event_t;
Queue<event_t, QUEUE_MAX> queue;
unsigned long count = 0;

void setup()
{
  Serial.begin(57600);
  Serial.println(F("SchedulerBenchmarkQueue: started"));
  Serial.flush();

  Scheduler.start(NULL, producer);
  Scheduler.start(NULL, consumer);
}

void loop()
{
  unsigned long start = millis();
  await(millis() - start >= 1000);
  Serial.print(millis());
  Serial.print(F(":loop::count="));
  Serial.println(count);
  count = 0;
}

void producer()
{
  event_t event = 0;
  while (1) queue.push(&event);
}

void consumer()
{
  event_t event;
  while (1) {
    queue.pull(&event);
    count += 1;
  }
}
