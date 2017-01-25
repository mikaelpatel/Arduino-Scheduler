/**
 * @file SchedulerBenchmark.ino
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
 * Benchmark Scheduler library.
 *
 * @section Results Arduino 1.6.7, Arduino Pro-Mini
 * SchedulerBenchmark: started
 * 1. Initiate scheduler and main task: 4 us
 * 2. Yield main task: 11.00 us
 * 3. Start a task: 20 us
 * 4. Yield between two tasks: 22.89 us
 * 5. Delay 10 ms and check increments: 10028:381, 26.32 us
 * 6. Start 8 tasks: 148:8, 18.50 us
 * 7. Yield and check increments: 120:9, 13.33 us
 * 8. Delay 10 ms and check increments: 10088:747, 13.50 us
 *
 * @section Results Arduino 1.6.7, Arduino Mega 2560
 * SchedulerBenchmark: started
 * 1. Initiate scheduler and main task: 4 us
 * 2. Yield main task: 12.64 us
 * 3. Start a task: 24 us
 * 4. Yield between two tasks: 26.16 us
 * 5. Delay 10 ms and check increments: 10028:338, 29.67 us
 * 6. Start 46 tasks: 860:46, 18.70 us
 * 7. Yield and check increments: 648:47, 13.79 us
 * 8. Delay 10 ms and check increments: 10436:752, 13.88 us
 *
 * @section Results Arduino 1.6.7, Arduino Due
 * SchedulerBenchmark: started
 * 1. Initiate scheduler and main task: 2 us
 * 2. Yield main task: 1.30 us
 * 3. Start a task: 3 us
 * 4. Yield between two tasks: 2.80 us
 * 5. Delay 10 ms and check increments: 9497:3186, 2.98 us
 * 6. Start 51 tasks: 112:51, 2.20 us
 * 7. Yield and check increments: 78:52, 1.50 us
 * 8. Delay 10 ms and check increments: 9874:6500, 1.52 us
 *
 * @section Results Arduino 1.6.7, Teensy 3.1 (72 MHz)
 * SchedulerBenchmark: started
 * 1. Initiate scheduler and main task: 1 us
 * 2. Yield main task: 1.10 us
 * 3. Start a task: 2 us
 * 4. Yield between two tasks: 2.51 us
 * 5. Delay 10 ms and check increments: 10003:3105, 3.22 us
 * 6. Start 24 tasks: 38:24, 1.58 us
 * 7. Yield and check increments: 30:25, 1.20 us
 * 8. Delay 10 ms and check increments: 10015:6950, 1.44 us
 */

#include <Scheduler.h>

// Check for SparkFun SAMD21 Breakout
#if defined(ARDUINO_ARCH_SAMD) && (USB_PID == 0x8D21)
#define Serial SerialUSB
#endif

int counter = 0;

void setup()
{
  unsigned long start, stop, us;
  int nr;

  Serial.begin(57600);
  while (!Serial);
  Serial.println(F("SchedulerBenchmark: started"));
  Serial.flush();

  // 1. Measure initiate scheduler
  start = micros();
  Scheduler.begin(256);
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
  us = stop - start;
  Serial.print(F("2. Yield main task: "));
  Serial.print(us / 10000.0);
  Serial.println(F(" us"));
  Serial.flush();

  // 3. Measure start of task
  start = micros();
  Scheduler.startLoop(incrementCounter);
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
  us = stop - start;
  if (counter != 10000) Serial.println(F("Error: counter"));
  Serial.print(F("4. Yield between two tasks: "));
  Serial.print(us / 10000.0);
  Serial.println(F(" us"));
  Serial.flush();

  // 5. Measure counter update during 10 ms delay
  counter = 0;
  start = micros();
  delay(10);
  stop = micros();
  us = stop - start;
  Serial.print(F("5. Delay 10 ms and check increments: "));
  Serial.print(us);
  Serial.print(F(":"));
  Serial.print(counter);
  Serial.print(F(", "));
  Serial.print(us / (float) counter);
  Serial.println(F(" us"));
  Serial.flush();

  // 6. Measure max number of tasks
  start = micros();
  nr = 0;
  while (Scheduler.startLoop(incrementCounter)) nr++;
  stop = micros();
  us = stop - start;
  Serial.print(F("6. Start "));
  Serial.print(nr);
  Serial.print(F(" tasks: "));
  Serial.print(us);
  Serial.print(F(":"));
  Serial.print(nr);
  Serial.print(F(", "));
  Serial.print(us / (float) nr);
  Serial.println(F(" us"));
  Serial.flush();

  // 7. Measure one yield cycle with max tasks
  counter = 0;
  start = micros();
  yield();
  stop = micros();
  us = stop - start;
  Serial.print(F("7. Yield and check increments: "));
  Serial.print(us);
  Serial.print(F(":"));
  Serial.print(counter);
  Serial.print(F(", "));
  Serial.print(us / (float) counter);
  Serial.println(F(" us"));
  Serial.flush();

  // 8. Measure counter update during 10 ms delay with max tasks
  counter = 0;
  start = micros();
  delay(10);
  stop = micros();
  us = stop - start;
  Serial.print(F("8. Delay 10 ms and check increments: "));
  Serial.print(us);
  Serial.print(F(":"));
  Serial.print(counter);
  Serial.print(F(", "));
  Serial.print(us / (float) counter);
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
