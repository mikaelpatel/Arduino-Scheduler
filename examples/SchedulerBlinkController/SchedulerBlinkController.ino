/**
 * @file SchedulerBlinkController.ino
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
 * This Arduino sketch uses the Scheduler library to control and
 * blink built-in LED. Send '1' to turn LED on and '0' to turn off.
 */

#include <Scheduler.h>

const int LED = 13;
volatile unsigned int period = 0;

void setup()
{
  Serial.begin(57600);
  Serial.println(F("SchedulerBlinkController: started"));

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  Serial.print(millis());
  Serial.println(F(":LED off"));
  Serial.flush();

  Scheduler.begin();
  Scheduler.start(controllerLoop);
  Scheduler.start(buttonLoop);
}

void loop()
{
  unsigned int ms = period;
  if (ms != 0) {
    digitalWrite(LED, HIGH);
    delay(ms);
    digitalWrite(LED, LOW);
    ms = period;
    if (ms != 0) delay(ms);
  }
  else {
    yield();
  }
}

void controllerLoop()
{
  unsigned int ms = 100;
  for (; ms < 1000; ms += 100) {
    delay(2000);
    if (period == 0) break;
    period = ms;
    Serial.print(millis());
    Serial.print(F(":period = "));
    Serial.println(ms);
  }
  for (; ms > 100; ms -= 100) {
    delay(2000);
    if (period == 0) break;
    period = ms;
    Serial.print(millis());
    Serial.print(F(":period = "));
    Serial.println(ms);
  }
}

void buttonLoop()
{
  if (Serial.available()) {
    int c = Serial.read();
    if (c == '0') {
      Serial.print(millis());
      Serial.println(F(":LED off"));
      period = 0;
    }
    else if (c == '1') {
      Serial.print(millis());
      Serial.println(F(":LED on"));
      period = 100;
    }
  }
  yield();
}
