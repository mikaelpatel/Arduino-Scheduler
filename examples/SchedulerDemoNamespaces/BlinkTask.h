/**
 * @file BlinkTask.h
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
 * BlinkTask demo as namespace.
 */

#ifndef BLINK_TASK_H
#define BLINK_TASK_H

#include <Scheduler.h>
#include "Trace.h"

namespace BlinkTask {

  const unsigned int PERIOD = 1000;
  const int LED = 13;

  void setup()
  {
    TRACE("stack=");
    Serial.println(Scheduler.stack());

    TRACELN("led output");
    pinMode(LED, OUTPUT);
  }

  void loop()
  {
    TRACE("stack=");
    Serial.println(Scheduler.stack());

    TRACELN("led off");
    digitalWrite(LED, LOW);
    delay(PERIOD);

    TRACELN("led on");
    digitalWrite(LED, HIGH);
    delay(PERIOD);
  }
};

#endif
