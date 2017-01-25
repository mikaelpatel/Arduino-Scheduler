/**
 * @file SchedulerBenchmarkSemaphore.ino
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
 * Benchmark Scheduler Semaphore.
 *
 * @section Results Arduino 1.6.7
 * Semaphore wait/signal per seconds.
 * Arduino Pro-Mini	26337 (37.97 us)
 * Arduino Mega 2560	23289 (42.94 us)
 * Arduino Due		196770 (5.08 us)
 */

#include <Scheduler.h>
#include <Scheduler/Semaphore.h>

// Check for SparkFun SAMD21 Breakout
#if defined(ARDUINO_ARCH_SAMD) && (USB_PID == 0x8D21)
#define Serial SerialUSB
#endif

Semaphore semaphore(0);
unsigned long count = 0;

void setup()
{
  Serial.begin(57600);
  while (!Serial);
  Serial.println(F("SchedulerBenchmarkSemaphore: started"));
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
  while (1) semaphore.signal();
}

void consumer()
{
  while (1) {
    semaphore.wait();
    count += 1;
  }
}
