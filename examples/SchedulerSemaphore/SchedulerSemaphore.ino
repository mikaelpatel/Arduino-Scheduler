/**
 * @file SchedulerSemaphore.ino
 * @version 1.0
 *
 * @section License
 * Copyright (C) Mikael Patel, 2015
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
 * Classical semafores used in this example to protect serial
 * output sequences.
 */

#include <Scheduler.h>

class Semaphore {
public:
  /**
   * Initiate semaphore with given count (non zero).
   * @param[in] count.
   */
  Semaphore(unsigned int count = 1) : m_count(count) {}

  /**
   * Wait for semaphore count.
   * @param[in] count.
   */
  void wait(unsigned int count = 1)
  {
    await(m_count >= count);
    m_count -= count;
  }

  /**
   * Signal semaphore count.
   * @param[in] count.
   */
  void signal(unsigned int count = 1)
  {
    m_count += count;
    yield();
  }

protected:
  volatile unsigned int m_count;
};

Semaphore mutex;

void setup()
{
  Serial.begin(57600);
  Serial.println(F("SchedulerSemaphore: started"));
  Scheduler.start(NULL, loop1);
}

void loop()
{
  static unsigned int i = 1;
  unsigned long start, stop, ms;

  start = millis();
  mutex.wait();
  {
    stop = millis();
    ms = stop - start;
    Serial.print(stop);
    Serial.print(F(":loop::i="));
    Serial.print(i);
    Serial.print(F(", wait="));
    Serial.println(ms);
    Serial.flush();
    delay(random(200));
  }
  mutex.signal();

  delay(500);
  i += 1;
}

void loop1()
{
  mutex.wait();
  {
    Serial.print(millis());
    Serial.println(F(":loop1::step-1"));
    delay(random(100));

    Serial.print(millis());
    Serial.println(F(":loop1::step-2"));
    delay(random(100));

    Serial.print(millis());
    Serial.println(F(":loop1::step-3"));
    Serial.flush();
    delay(random(100));
  }
  mutex.signal();
}
