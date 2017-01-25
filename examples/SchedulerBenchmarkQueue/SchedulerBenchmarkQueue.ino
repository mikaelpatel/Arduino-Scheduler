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
 * Events per second (push/pull). QUEUE_MAX 2, 8
 * Arduino Pro-Mini	24029 (41.62 us), 92638 (10.79 us)
 * Arduino Mega 2560	21466 (46.59 us), 86919 (11.50 us)
 * Arduino Due		183411 (5.45 us), 677502 (1.48 us)
 */

#include <Scheduler.h>
#include <Scheduler/Queue.h>

// Check for SparkFun SAMD21 Breakout
#if defined(ARDUINO_ARCH_SAMD) && (USB_PID == 0x8D21)
#define Serial SerialUSB
#endif

// const unsigned int QUEUE_MAX = 2;
const unsigned int QUEUE_MAX = 8;
typedef int event_t;
Queue<event_t, QUEUE_MAX> queue;
unsigned long count = 0;

void setup()
{
  Serial.begin(57600);
  while (!Serial);
  Serial.println(F("SchedulerBenchmarkQueue: started"));
  Serial.flush();

  Scheduler.startLoop(producer);
  Scheduler.startLoop(consumer);
}

void loop()
{
  unsigned long start = millis();
  await(millis() - start >= 1000);
  Serial.print(count);
  Serial.print(F(" ("));
  Serial.print(1000000.0 / count);
  Serial.println(F(" us)"));
  Serial.flush();
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
