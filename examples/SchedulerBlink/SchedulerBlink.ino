/**
 * @file SchedulerBlink.ino
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
 * This Arduino sketch uses the Scheduler library to blink three
 * LEDs with different speed.
 */

#include <Scheduler.h>

const int led1 = 13;
const int led2 = 12;
const int led3 = 11;

void setup()
{
  // Initiate pins
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  // Initiate scheduler and threads
  Scheduler.begin();
  Scheduler.start(loop2);
  Scheduler.start(loop3);
}

void loop()
{
  blink(led1, 500);
}

void loop2()
{
  blink(led2, 250);
}

void loop3()
{
  blink(led3, 1000);
}

void blink(int pin, unsigned int ms)
{
  digitalWrite(pin, HIGH);
  delay(ms);
  digitalWrite(pin, LOW);
  delay(ms);
}

