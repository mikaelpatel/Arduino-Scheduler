/**
 * @file SchedulerBlink.ino
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
 * This Arduino sketch uses the Scheduler library to blink three
 * LEDs with different speed. Each LED is controlled by a separate
 * task.
 */

#include <Scheduler.h>

void blink(int pin, unsigned int ms)
{
  digitalWrite(pin, HIGH);
  delay(ms);
  digitalWrite(pin, LOW);
  delay(ms);
}

void setup()
{
  Scheduler.start(setup1, loop1);
  Scheduler.start(setup2, loop2);
  Scheduler.start(setup3, loop3);
}

void loop()
{
  yield();
}

const int led1 = 11;

void setup1()
{
  pinMode(led1, OUTPUT);
}

void loop1()
{
  blink(led1, 500);
}

const int led2 = 12;

void setup2()
{
  pinMode(led2, OUTPUT);
}

void loop2()
{
  blink(led2, 250);
}

const int led3 = 13;

void setup3()
{
  pinMode(led3, OUTPUT);
}

void loop3()
{
  blink(led3, 1000);
}

