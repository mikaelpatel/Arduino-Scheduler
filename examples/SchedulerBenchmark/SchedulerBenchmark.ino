/**
 * @file SchedulerBenckmark.ino
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
 *
 * @section Description
 * Benchmark Scheduler library.
 *  1. Initiate scheduler and main thread: 4 us
 *  2. Yield main thread: 12 us
 *  3. Start a thread: 24 us
 *  4. Yield between threads: 25 us
 *  5. Delay 10 ms and check increments: 339, 29 us
 * Arduino 1.6.7, Mega 2560 (clone).
 */

#include <Scheduler.h>

int counter = 0;

void setup()
{
  unsigned long start, stop, us;

  Serial.begin(57600);
  Serial.println(F("SchedulerBenckmark: started"));
  Serial.flush();

  start = micros();
  Scheduler.begin();
  stop = micros();
  us = stop - start;
  Serial.print(F("1. Initiate scheduler and main thread: "));
  Serial.print(us);
  Serial.println(F(" us"));
  Serial.flush();

  start = micros();
  for (int i = 0; i < 10000; i++)
    yield();
  stop = micros();
  us = (stop - start) / 10000;
  Serial.print(F("2. Yield main thread: "));
  Serial.print(us);
  Serial.println(F(" us"));

  start = micros();
  Scheduler.start(incrementCounter);
  stop = micros();
  us = stop - start;
  Serial.print(F("3. Start a thread: "));
  Serial.print(us);
  Serial.println(F(" us"));
  Serial.flush();

  start = micros();
  for (int i = 0; i < 10000; i++)
    yield();
  stop = micros();
  us = (stop - start) / 10000;
  if (counter != 10000) Serial.println(F("Error: counter"));
  Serial.print(F("4. Yield between threads: "));
  Serial.print(us);
  Serial.println(F(" us"));
  Serial.flush();

  counter = 0;
  delay(10);
  Serial.print(F("5. Delay 10 ms and check increments: "));
  Serial.print(counter);
  Serial.print(F(", "));
  Serial.print(10000 / counter);
  Serial.println(F(" us"));
  Serial.flush();
}

void loop()
{
}

void incrementCounter()
{
  counter += 1;
  yield();
}

