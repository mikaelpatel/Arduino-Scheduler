/**
 * @file Blink.h
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
 * Blink as a template class.
 */

#ifndef BLINK_H
#define BLINK_H

template<int PIN, unsigned int MS>
class Blink {
public:
  static void setup()
  {
    pinMode(PIN, OUTPUT);
  }
  static void loop()
  {
    digitalWrite(PIN, HIGH);
    delay(MS);
    digitalWrite(PIN, LOW);
    delay(MS);
  }
};

#endif
