/**
 * @file SchedulerBenchmark.ino
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
 *
 * @section Results Arduino 1.6.7, Mega 2560
 * SchedulerBenchmark: started
 * 1. Initiate scheduler and main task: 4 us
 * 2. Yield main task: 12 us
 * 3. Start a task: 20 us
 * 4. Yield between two tasks: 26 us
 * 5. Delay 10 ms and check increments: 338, 10024:29 us
 * 6. Start 58 tasks: 980:16 us
 * 7. Yield and check increments: 59, 800:13 us
 * 8. Delay 10 ms and check increments: 767, 10592:13 us
 *
 * @section Results Arduino 1.6.7, Pro-Mini
 * SchedulerBenchmark: started
 * 1. Initiate scheduler and main task: 4 us
 * 2. Yield main task: 11 us
 * 3. Start a task: 20 us
 * 4. Yield between two tasks: 22 us
 * 5. Delay 10 ms and check increments: 381, 10032:26 us
 * 6. Start 10 tasks: 164:16 us
 * 7. Yield and check increments: 11, 144:13 us
 * 8. Delay 10 ms and check increments: 759, 10028:13 us
 */

#include <Scheduler.h>

int counter = 0;

void setup()
{
  unsigned long start, stop, us;
  int nr;

  Serial.begin(57600);
  Serial.println(F("SchedulerBenchmark: started"));
  Serial.flush();

  // 1. Measure initiate scheduler
  start = micros();
  Scheduler.begin(128);
  stop = micros();
  us = stop - start;
  Serial.print(F("1. Initiate scheduler and main task: "));
  Serial.print(us);
  Serial.println(F(" us"));
  Serial.flush();

  // 2. Measure yield with single task
  start = micros();
  for (int i = 0; i < 10000; i++)
    yield();
  stop = micros();
  us = (stop - start) / 10000;
  Serial.print(F("2. Yield main task: "));
  Serial.print(us);
  Serial.println(F(" us"));
  Serial.flush();

  // 3. Measure start of task
  start = micros();
  Scheduler.start(NULL, incrementCounter);
  stop = micros();
  us = stop - start;
  Serial.print(F("3. Start a task: "));
  Serial.print(us);
  Serial.println(F(" us"));
  Serial.flush();

  // 4. Measure yield between two tasks
  start = micros();
  for (int i = 0; i < 10000; i++)
    yield();
  stop = micros();
  us = (stop - start) / 10000;
  if (counter != 10000) Serial.println(F("Error: counter"));
  Serial.print(F("4. Yield between two tasks: "));
  Serial.print(us);
  Serial.println(F(" us"));
  Serial.flush();

  // 5. Measure counter update during 10 ms delay
  counter = 0;
  start = micros();
  delay(10);
  stop = micros();
  us = stop - start;
  Serial.print(F("5. Delay 10 ms and check increments: "));
  Serial.print(counter);
  Serial.print(F(", "));
  Serial.print(us);
  Serial.print(F(":"));
  Serial.print(us / counter);
  Serial.println(F(" us"));
  Serial.flush();

  // 6. Measure max number of tasks
  start = micros();
  nr = 0;
  while (Scheduler.start(NULL, incrementCounter)) nr++;
  stop = micros();
  us = stop - start;
  Serial.print(F("6. Start "));
  Serial.print(nr);
  Serial.print(F(" tasks: "));
  Serial.print(us);
  Serial.print(F(":"));
  Serial.print(us / nr);
  Serial.println(F(" us"));
  Serial.flush();

  // 7. Measure one yield cycle with max tasks
  counter = 0;
  start = micros();
  yield();
  stop = micros();
  us = stop - start;
  Serial.print(F("7. Yield and check increments: "));
  Serial.print(counter);
  Serial.print(F(", "));
  Serial.print(us);
  Serial.print(F(":"));
  Serial.print(us / counter);
  Serial.println(F(" us"));
  Serial.flush();

  // 8. Measure counter update during 10 ms delay with max tasks
  counter = 0;
  start = micros();
  delay(10);
  stop = micros();
  us = stop - start;
  Serial.print(F("8. Delay 10 ms and check increments: "));
  Serial.print(counter);
  Serial.print(F(", "));
  Serial.print(us);
  Serial.print(F(":"));
  Serial.print(us / counter);
  Serial.println(F(" us"));
  Serial.flush();
}

void loop()
{
  yield();
}

void incrementCounter()
{
  counter += 1;
  yield();
}

