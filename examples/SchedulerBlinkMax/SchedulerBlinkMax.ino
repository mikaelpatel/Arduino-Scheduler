/**
 * @file SchedulerBlinkMax.ino
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
 * Blink with max number of pins; 64 bytes per task.
 */

#include <Scheduler.h>

int next_pin = 2;
int stack;

void blink()
{
  // Allocate a pin and set output mode
  int pin = next_pin++;
  pinMode(pin, OUTPUT);

  // Capture stack available
  stack = Scheduler.stack();

  // Toggle the pin on and off
  while (1) {
    digitalWrite(pin, HIGH);
    delay(500);
    digitalWrite(pin, LOW);
    delay(500);
  }
}

void setup()
{
  Serial.begin(57600);
  while (!Serial);
  Serial.println(F("SchedulerBlinkMax: started"));
  Serial.flush();

  // Start a blink task for each pin (use 64 byte stack)
  int tasks = 0;
  const int TASKS_MAX = NUM_DIGITAL_PINS - 2;
  while (tasks < TASKS_MAX && Scheduler.start(NULL, blink, 64)) tasks++;
  Serial.print(millis());
  Serial.print(F(":setup::task="));
  Serial.println(tasks);

  // Run blink tasks startup and print stack available
  yield();
  Serial.print(millis());
  Serial.print(F(":setup::stack="));
  Serial.println(stack);
}

void loop()
{
  // Print the iteration count
  static unsigned int i = 0;
  Serial.print(millis());
  Serial.print(F(":loop::i="));
  Serial.println(i++);

  // Delay gracefully
  unsigned long start = millis();
  await((millis() - start) >= 1000);
}
