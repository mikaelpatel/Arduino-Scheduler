/**
 * @file Watchdog.h
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
 * Scheduler watchdog template class.
 */

#ifndef BLINK_H
#define BLINK_H

template<unsigned int DEADLINE, void (*ALARM)(unsigned long ms)>
class Watchdog {
public:
  static void setup()
  {
  }
  static void loop()
  {
    unsigned long start = millis();
    yield();
    unsigned long ms = millis() - start;
    if (ms > DEADLINE) ALARM(ms);
  }
};

#endif
