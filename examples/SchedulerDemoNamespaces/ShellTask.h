/**
 * @file ShellTask.h
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
 * ShellTask demo as namespace.
 */

#ifndef SHELL_TASK_H
#define SHELL_TASK_H

#include <Scheduler.h>
#include "Trace.h"

namespace ShellTask {

  const size_t BUF_MAX = 64;
  static char* buf = NULL;

  void setup()
  {
    TRACE("stack=");
    Serial.println(Scheduler.stack());
  }

  void loop()
  {
    // Check for buffer allocation
    if (buf == NULL) {
      buf = (char*) malloc(BUF_MAX);
      TRACE("malloc:buf=0x");
      Serial.println((int) buf, HEX);
      if (buf == NULL) {
	delay(1000);
	return;
      }
    }

    // Read line and yield while waiting for characters
    // Capture wait time and number of yields
    char* bp = buf;
    unsigned long yields = 0;
    unsigned long start = millis();
    int c;
    TRACE("stack=");
    Serial.println(Scheduler.stack());
    while ((c = Serial.read()) != '\n') {
      if (c > 0)
	*bp++ = c;
      else {
	yields += 1;
	yield();
      }
    }
    *bp = 0;

    // Print wait time, number of yields and line
    unsigned long stop = millis();
    unsigned long ms = stop - start;
    TRACE("yields=");
    Serial.print(yields);
    Serial.print(F(",ms="));
    Serial.print(ms);
    Serial.print(F(",buf="));
    Serial.println(buf);

    // Check for buffer free command
    if (!strcmp_P(buf, (const char*) F("free"))) {
      TRACE("free:buf=0x");
      Serial.println((int) buf, HEX);
      free(buf);
      delay(500);
      buf = NULL;
    }
  }
};
#endif
