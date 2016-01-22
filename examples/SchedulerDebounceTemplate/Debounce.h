/**
 * @file Debounce.h
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
 * Debounce task template class.
 */

#ifndef DEBOUNCE_H
#define DEBOUNCE_H

template<int PIN, void (*CALLBACK)()>
class Debounce {
public:
  static void setup()
  {
    pinMode(PIN, INPUT_PULLUP);
  }
  static void loop()
  {
    const unsigned int DEBOUNCE = 50;
    int state = digitalRead(PIN);
    while (1) {
      await(digitalRead(PIN) != state);
      unsigned int start = millis();
      if (state) CALLBACK();
      unsigned int ms = millis() - start;
      if (ms < DEBOUNCE) delay(DEBOUNCE - ms);
      state = !state;
    }
  }
};

#endif
