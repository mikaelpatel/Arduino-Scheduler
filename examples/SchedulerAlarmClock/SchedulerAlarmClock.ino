/**
 * @file SchedulerAlarmClock.ino
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
 * This Arduino sketch shows how to use the Scheduler library.
 */

#include <Scheduler.h>

#include "Clock.h"
#include "Alarm.h"

// Check for SparkFun SAMD21 Breakout
#if defined(ARDUINO_ARCH_SAMD) && (USB_PID == 0x8D21)
#define Serial SerialUSB
#endif

void setup()
{
  Serial.begin(57600);
  while (!Serial);
  Serial.println(F("SchedulerAlarmClock: started"));

  // Initiate tasks
  Scheduler.start(Clock::setup, Clock::loop);
  Scheduler.start(Alarm::setup, Alarm::loop);

  // Set alarm time
  Alarm::time = 15;
}

void loop()
{
  unsigned long seconds = Clock::seconds;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  Serial.print(millis());
  Serial.print(':');
  Serial.print(hours);
  Serial.print(':');
  Serial.print(minutes % 60);
  Serial.print(':');
  Serial.println(seconds % 60);
  await(seconds != Clock::seconds);
}

