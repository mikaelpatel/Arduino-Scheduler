/**
 * @file SchedulerDebounce.ino
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
 * Software debounce a button.
 */

#include <Scheduler.h>

// Check for SparkFun SAMD21 Breakout
#if defined(ARDUINO_ARCH_SAMD) && (USB_PID == 0x8D21)
#define Serial SerialUSB
#endif

unsigned int count = 0;

void setup()
{
  Serial.begin(57600);
  while (!Serial);
  Serial.println(F("SchedulerDebounce: started"));
  Scheduler.start(debounceSetup, debounceLoop);
}

void loop()
{
  Serial.print(millis());
  Serial.print(F(":loop::count="));
  Serial.println(count);
  delay(1000);
}

const int BUTTON = 4;
volatile int state;

void debounceSetup()
{
  pinMode(BUTTON, INPUT_PULLUP);
}

void debounceLoop()
{
  const unsigned int DEBOUNCE = 50;
  state = digitalRead(BUTTON);
  while (1) {
    await(digitalRead(BUTTON) != state);
    state = !state;
    if (state)
      buttonUp();
    else
      buttonDown();
    delay(DEBOUNCE);
  }
}

void buttonDown()
{
  Serial.print(millis());
  Serial.println(F(":buttonDown"));
  count += 1;
}

void buttonUp()
{
  Serial.print(millis());
  Serial.println(F(":buttonUp"));
}
