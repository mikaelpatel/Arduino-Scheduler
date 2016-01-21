/**
 * @file SchedulerBlinkTemplateClass.ino
 * @version 1.0
 *
 * @section License
 * Copyright (C) Mikael Patel, 2015-2016
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
 * This Arduino sketch uses the Scheduler library. Rewrite of
 * SchedulerBlink with template class.
 */

#include <Scheduler.h>
#include "Blink.h"

// Blink task definitions with pin and delay period
Blink<11, 500> blink1;
Blink<12, 250> blink2;
Blink<13, 1000> blink3;

void setup()
{
  Scheduler.start(blink1.setup, blink1.loop);
  Scheduler.start(blink2.setup, blink2.loop);
  Scheduler.start(blink3.setup, blink3.loop);
}

void loop()
{
  yield();
}
