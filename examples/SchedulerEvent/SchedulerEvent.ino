/**
 * @file SchedulerEvent.ino
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
 * Pass events between tasks.
 */

#include <Scheduler.h>

// Check for SparkFun SAMD21 Breakout
#if defined(ARDUINO_ARCH_SAMD) && (USB_PID == 0x8D21)
#define Serial SerialUSB
#endif

class Event {
public:
  /**
   * Initiate task event.
   */
  Event() : m_event(0) {}

  /**
   * Wait for task event. Returns event code (non zero).
   * Typically called by event handler.
   * @return event.
   */
  unsigned int wait()
  {
    await(m_event != 0);
    return (m_event);
  }

  /**
   * Signal task event. Wait for ongoing event handler,
   * assign new event code and yield.
   * @param[in] value event code (non zero).
   */
  void signal(unsigned int value)
  {
    await(m_event == 0);
    m_event = value;
    yield();
  }

  /**
   * Clear event code to mark that it was completed.
   */
  void clear()
  {
    m_event = 0;
  }

protected:
  volatile unsigned int m_event;
};

Event event;

void setup()
{
  Serial.begin(57600);
  while (!Serial);
  Serial.println(F("SchedulerEvent: started"));
  Scheduler.startLoop(eventHandler);
}

void loop()
{
  static unsigned int i = 1;
  Serial.print(millis());
  Serial.print(F(":loop::event="));
  Serial.println(i);
  delay(1000);
  event.signal(i);
  i += 1;
  if (i == 0) i = 1;
}

void eventHandler()
{
  unsigned int value = event.wait();
  Serial.print(millis());
  Serial.print(F(":eventHandler::event="));
  Serial.println(value);
  delay(200);
  event.clear();
}
