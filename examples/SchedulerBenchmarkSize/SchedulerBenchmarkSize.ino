/**
 * @file SchedulerBenchmarkSize.ino
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
 * Benchmark size of Scheduler library.
 *
 * @section Results Arduino 1.6.7
 * Board		Baseline 	Total		Size
 * Arduino Pro-Mini	1810/200 	2356/242	546/42
 * Arduino Mega 2560	2098/200	2646/244	548/44
 * Arduino Due		10204/--	10428/--  	224/--
 * Teensy 3.1 (72 MHz)	16312/4644	10556/4304	???/???
 */

// Comment for baseline
#define TASKS

#ifdef TASKS
#include <Scheduler.h>

// Check for SparkFun SAMD21 Breakout
#if defined(ARDUINO_ARCH_SAMD) && (USB_PID == 0x8D21)
#define Serial SerialUSB
#endif

#endif

void setup()
{
  Serial.begin(57600);
  while (!Serial);
  Serial.println(F("SchedulerBenchmarkSize: started"));
  Serial.flush();

#ifdef TASKS
  Scheduler.startLoop(loop);
#endif
}

void loop()
{
  yield();
}
