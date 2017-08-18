/**
 * @file Scheduler/Semaphore.h
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
 */

#ifndef SCHEDULER_SEMAPHORE_H
#define SCHEDULER_SEMAPHORE_H

#include <Scheduler.h>

class Semaphore {
public:
  /**
   * Initiate semaphore with given count.
   * @param[in] count (default mutex).
   */
  Semaphore(unsigned int count = 1) : m_count(count) {}

  /**
   * Wait for semaphore count.
   * @param[in] count (default mutex).
   */
  void wait(unsigned int count = 1)
  {
    await(m_count >= count);
    m_count -= count;
  }

  /**
   * Signal semaphore count.
   * @param[in] count (default mutex).
   */
  void signal(unsigned int count = 1)
  {
    m_count += count;
    Scheduler.yield();
  }

protected:
  volatile unsigned int m_count;
};

#endif
