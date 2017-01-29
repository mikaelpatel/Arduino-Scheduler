/**
 * @file SchedulerTaskMax.ino
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
 * This Arduino sketch uses the Scheduler library.
 * Schedule max number of counter tasks.
 */

#include <Scheduler.h>

// Check for SparkFun SAMD21 Breakout
#if defined(ARDUINO_ARCH_SAMD) && (USB_PID == 0x8D21)
#define Serial SerialUSB
#endif

#if defined(ARDUINO_ARCH_AVR)
const size_t STACK_SIZE = 64;
#else
const size_t STACK_SIZE = 128;
#endif

unsigned long count = 0L;

void setup()
{
  Serial.begin(57600);
  while (!Serial);
  Serial.println(F("SchedulerTaskMax: started"));
  Serial.flush();

  int tasks = 0;
  while (Scheduler.startLoop(counter, STACK_SIZE)) tasks++;
  Serial.print(millis());
  Serial.print(F(":setup::task="));
  Serial.println(tasks);
  yield();
  Serial.print(millis());
  Serial.print(F(":setup::count="));
  Serial.println(count);
}

void loop()
{
  count = 0;
  unsigned long start = millis();
  await((millis() - start) >= 1000);
  Serial.print(millis() - start);
  Serial.print(F(":loop::count="));
  Serial.println(count);
}

void counter()
{
  count += 1;
  yield();
}
