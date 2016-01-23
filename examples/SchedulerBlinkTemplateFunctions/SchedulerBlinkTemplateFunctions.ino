/**
 * @file SchedulerBlinkTemplateFunctions.ino
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
 * This Arduino sketch uses the Scheduler library. Rewrite of
 * SchedulerBlink with template functions for Blink setup and loop.
 */

#include <Scheduler.h>

template<int pin> void setupBlink()
{
  pinMode(pin, OUTPUT);
}

template<int pin, unsigned int ms> void loopBlink()
{
  digitalWrite(pin, HIGH);
  delay(ms);
  digitalWrite(pin, LOW);
  delay(ms);
}

void setup()
{
  setupBlink<13>();
  Scheduler.start(setupBlink<10>, loopBlink<10,250>);
  Scheduler.start(setupBlink<11>, loopBlink<11,500>);
  Scheduler.start(setupBlink<12>, loopBlink<12,750>);
}

void loop()
{
  loopBlink<13,1000>();
}
