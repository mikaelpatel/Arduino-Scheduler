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
 * Messages per second (send/recv).
 * Arduino Pro-Mini	24328 (41.10 us)
 * Arduino Mega 2560	21704 (46.07 us)
 * Arduino Due	       168344 (5.94 us)
 */

#include <Scheduler.h>
#include <Scheduler/Channel.h>

const size_t PAYLOAD_MAX = 1;
struct msg_t {
  int payload[PAYLOAD_MAX];
};

Channel chan;
unsigned long count = 0;

void setup()
{
  Serial.begin(57600);
  while (!Serial);
  Serial.println(F("SchedulerBenchmarkChannel: started"));
  Serial.flush();

  Scheduler.start(NULL, producer);
  Scheduler.start(NULL, consumer);
}

void loop()
{
  unsigned long start = millis();
  await(millis() - start >= 1000);
  Serial.print(millis());
  Serial.print(F(":loop::count="));
  Serial.print(count);
  Serial.print(F(", us="));
  Serial.println(1000000.0 / count);
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
