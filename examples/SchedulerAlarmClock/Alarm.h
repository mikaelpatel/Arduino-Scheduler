/**
 * @file Alarm.h
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
 * Alarm task.
 */

#ifndef ALARM_H
#define ALARM_H

#include "Clock.h"

namespace Alarm {

  unsigned long time = 0;

  void setup()
  {
  }

  void loop()
  {
    unsigned long seconds = Clock::seconds;
    if ((time != 0) && (seconds == time)) {
      Serial.println(F("alarm"));
      time += 20;
    }
    await(seconds != Clock::seconds);
  }
};
#endif
