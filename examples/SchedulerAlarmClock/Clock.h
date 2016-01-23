/**
 * @file Clock.h
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
 * Clock as a task.
 */

#ifndef CLOCK_H
#define CLOCK_H

namespace Clock {

  volatile unsigned long seconds = 0;

  void setup()
  {
  }

  void loop()
  {
    unsigned long start = millis();
    await(millis() - start >= 1000);
    seconds += 1;
  }
};

#endif
