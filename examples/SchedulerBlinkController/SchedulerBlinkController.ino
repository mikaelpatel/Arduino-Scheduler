/**
 * @file SchedulerBlinkController.ino
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
 * This Arduino sketch uses the Scheduler library to control and
 * blink built-in LED. Send '1' to turn LED on and '0' to turn off.
 */

#include <Scheduler.h>

// Check for SparkFun SAMD21 Breakout
#if defined(ARDUINO_ARCH_SAMD) && (USB_PID == 0x8D21)
#define Serial SerialUSB
#endif

const int LED = 13;
volatile unsigned int period = 0;

void setup()
{
  Serial.begin(57600);
  while (!Serial);
  Serial.println(F("SchedulerBlinkController: started"));
  Serial.println(F("0 - stop, 1 - start controller"));

  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  Serial.print(millis());
  Serial.println(F(":LED off"));
  Serial.flush();

  Scheduler.startLoop(controllerLoop);
  Scheduler.startLoop(buttonLoop);
}

void loop()
{
  await(period != 0);
  digitalWrite(LED, HIGH);
  delay(period);
  digitalWrite(LED, LOW);
  if (period == 0) return;
  delay(period);
}

void controllerLoop()
{
  unsigned int ms = 100;
  for (; ms < 1000; ms += 100) {
    delay(2000);
    if (period == 0) return;
    period = ms;
    Serial.print(millis());
    Serial.print(F(":period = "));
    Serial.println(ms);
  }
  for (; ms > 100; ms -= 100) {
    delay(2000);
    if (period == 0) return;
    period = ms;
    Serial.print(millis());
    Serial.print(F(":period = "));
    Serial.println(ms);
  }
}

void buttonLoop()
{
  unsigned long start = millis();
  await(Serial.available() != 0);
  unsigned long stop = millis();
  unsigned long ms = stop - start;
  int c = Serial.read();
  if (c == '0' && period != 0) {
    Serial.print(stop);
    Serial.print(':');
    Serial.print(ms);
    Serial.println(F(":LED off"));
    period = 0;
  }
  else if (c == '1' && period == 0) {
    Serial.print(stop);
    Serial.print(':');
    Serial.print(ms);
    Serial.println(F(":LED on"));
    period = 100;
  }
}
