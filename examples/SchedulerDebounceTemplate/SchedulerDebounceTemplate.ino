/**
 * @file SchedulerDebounceTemplate.ino
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
 * This Arduino sketch shows how to use the Scheduler library;
 * Rewrite of SchedulerDebounce with Debounce task template class.
 */

#include <Scheduler.h>
#include "Debounce.h"

// Check for SparkFun SAMD21 Breakout
#if defined(ARDUINO_ARCH_SAMD) && (USB_PID == 0x8D21)
#define Serial SerialUSB
#endif

const int BUTTON = 4;
unsigned int count = 0;

void buttonDown()
{
  Serial.print(millis());
  Serial.println(F(":buttonDown"));
  count += 1;
}

Debounce<BUTTON, buttonDown> button;

void setup()
{
  Serial.begin(57600);
  while (!Serial);
  Serial.println(F("SchedulerDebounceTemplate: started"));
  Scheduler.start(button.setup, button.loop);
}

void loop()
{
  Serial.print(millis());
  Serial.print(F(":loop::count="));
  Serial.println(count);
  delay(1000);
}
