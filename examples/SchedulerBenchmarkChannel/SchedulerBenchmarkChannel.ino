/**
 * @file SchedulerBenchmarkChannel.ino
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
 * Benchmark Scheduler Channel.
 *
 * @section Results Arduino 1.6.7
 * Messages per second (send/recv). PAYLOAD 1, 8
 * Arduino Pro-Mini	23921 (41.80 us), 22002 (45.45 us)
 * Arduino Mega 2560	21379 (46.77 us), 19834 (50.41 us)
 * Arduino Due		165322 (6.05 us), 160893 (6.22 us)
 */

#include <Scheduler.h>
#include <Scheduler/Channel.h>

// Check for SparkFun SAMD21 Breakout
#if defined(ARDUINO_ARCH_SAMD) && (USB_PID == 0x8D21)
#define Serial SerialUSB
#endif

// const size_t PAYLOAD_MAX = 1;
const size_t PAYLOAD_MAX = 8;
struct msg_t {
  uint8_t payload[PAYLOAD_MAX];
};

Channel chan;
unsigned long count = 0;

void setup()
{
  Serial.begin(57600);
  while (!Serial);
  Serial.println(F("SchedulerBenchmarkChannel: started"));
  Serial.flush();

  Scheduler.startLoop(producer);
  Scheduler.startLoop(consumer);
}

void loop()
{
  unsigned long start = millis();
  await(millis() - start >= 1000);
  Serial.print(count);
  Serial.print(F(" ("));
  Serial.print(1000000.0 / count);
  Serial.println(F(" us)"));
  Serial.flush();
  count = 0;
}

void producer()
{
  msg_t msg;
  while (1)
    chan.send(&msg, sizeof(msg));
}

void consumer()
{
  msg_t msg;
  while (1) {
    chan.recv(&msg, sizeof(msg));
    count += 1;
  }
}
