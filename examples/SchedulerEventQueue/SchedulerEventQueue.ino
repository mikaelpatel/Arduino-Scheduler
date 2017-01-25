/**
 * @file SchedulerEventQueue.ino
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
 * Queue events between tasks.
 */

#include <Scheduler.h>
#include <Scheduler/Queue.h>

// Check for SparkFun SAMD21 Breakout
#if defined(ARDUINO_ARCH_SAMD) && (USB_PID == 0x8D21)
#define Serial SerialUSB
#endif

Queue<unsigned int, 8> eventq;

void setup()
{
  Serial.begin(57600);
  while (!Serial);
  Serial.println(F("SchedulerEventQueue: started"));
  Serial.flush();

  // Start two event handlers
  Scheduler.startLoop(eventHandler);
  Scheduler.startLoop(eventHandler);
}

void loop()
{
  // Push events
  static unsigned int event = 0;
  Serial.print(millis());
  Serial.print(F(":loop::push event="));
  Serial.println(event);
  eventq.push(&event);
  event += 1;

  // Random processing time
  delay(random(500));
}

void eventHandler()
{
  // Pull events
  unsigned int event;
  eventq.pull(&event);
  Serial.print(millis());
  Serial.print(F(":eventHandler#"));
  Serial.print((int) &event);
  Serial.print(F("::pull event="));
  Serial.println(event);

  // Random service time
  delay(random(1000));
}
