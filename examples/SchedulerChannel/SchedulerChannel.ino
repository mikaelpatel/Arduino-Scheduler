/**
 * @file SchedulerChannel.ino
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
 * This Arduino sketch shows how to use the Scheduler library;
 * Message channel between tasks; two senders and one receiver.
 */

#include <Scheduler.h>
#include <Scheduler/Channel.h>

// Check for SparkFun SAMD21 Breakout
#if defined(ARDUINO_ARCH_SAMD) && (USB_PID == 0x8D21)
#define Serial SerialUSB
#endif

Channel chan;

void setup()
{
  Serial.begin(57600);
  while (!Serial);
  Serial.println(F("SchedulerChannel: started"));
  Serial.flush();

  Scheduler.startLoop(messageSender);
  Scheduler.startLoop(messageSender);
  Scheduler.startLoop(messageReceiver);
  Scheduler.startLoop(messageReceiver);
}

void loop()
{
  static unsigned int count = 0;
  Serial.print(millis());
  Serial.print(F(":loop:count="));
  Serial.println(count++);
  unsigned long start = millis();
  await(millis() - start >= 1000);
}

struct msg_t {
  int ref;
  unsigned int nr;
};

void messageSender()
{
  msg_t msg = { (int) &msg, 0 };
  while (1) {
    Serial.print(millis());
    Serial.print(F(":messageSender#"));
    Serial.print(msg.ref);
    Serial.print(F("::send msg.nr="));
    Serial.println(msg.nr);
    chan.send(&msg, sizeof(msg));
    msg.nr += 1;
    delay(random(500));
  }
}

void messageReceiver()
{
  msg_t msg;
  chan.recv(&msg, sizeof(msg));
  Serial.print(millis());
  Serial.print(F(":messageReceiver#"));
  Serial.print((int) &msg);
  Serial.print(F("::receive msg.ref="));
  Serial.print(msg.ref);
  Serial.print(F(", msg.nr="));
  Serial.println(msg.nr);
  delay(random(1000));
}
