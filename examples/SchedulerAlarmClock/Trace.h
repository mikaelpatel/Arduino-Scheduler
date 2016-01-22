/**
 * @file Trace.h
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
 * Some serial trace support macros.
 */

#ifndef TRACE_H
#define TRACE_H

#define TRACE(msg)					\
  do {							\
    Serial.print(millis());				\
    Serial.print(':');					\
    Serial.print(__PRETTY_FUNCTION__);			\
    Serial.print(':');					\
    Serial.print(F(msg));				\
  } while (0)

#define TRACELN(msg)					\
  do {							\
    TRACE(msg);						\
    Serial.println();					\
  } while (0)

#endif
