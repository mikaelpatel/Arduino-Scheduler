/**
 * @file SchedulerSize.ino
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
 * Size of Scheduler library.
 *
 * @section Results Arduino 1.6.7
 * Board		Baseline 	Total		Size
 * Arduino Pro-Mini	1802/200 	2348/242	546/42
 * Arduino Mega 2560	2090/200	2638/244	548/44
 * Arduino Due		10196/--	10420/--  	224/--
 */

#define TASKS

#ifdef TASKS
#include <Scheduler.h>
#endif

void setup()
{
  Serial.begin(57600);
  Serial.println(F("SchedulerSize: started"));
  Serial.flush();

#ifdef TASKS
  Scheduler.start(NULL, loop);
#endif
}

void loop()
{
  yield();
}


