/**
 * @file Scheduler/Queue.h
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

#ifndef SCHEDULER_QUEUE_H
#define SCHEDULER_QUEUE_H

/**
 * Template class for ring-buffer of queueing data elements.
 * @param[in] T element class.
 * @param[in] nmemb number of elements in queue.
 */
template <class T, uint8_t NMEMB>
class Queue {
  static_assert(NMEMB && !(NMEMB & (NMEMB - 1)), "Queue::NMEMB should be power of 2");
public:
  /**
   * Construct a ring-buffer queue with given number of members and
   * member type.
   */
  Queue() :
    m_put(0),
    m_get(0)
  {}

  /**
   * Return number of elements in queue.
   * @return available elements.
   */
  unsigned int available() const
    __attribute__((always_inline))
  {
    return ((NMEMB + m_put - m_get) & MASK);
  }

  /**
   * Number of elements room in queue.
   * @return room for elements.
   */
  unsigned int room() const
    __attribute__((always_inline))
  {
    return ((NMEMB - m_put + m_get - 1) & MASK);
  }

  /**
   * Push given data into queue. Will yield until room is available.
   * @param[in] data pointer to data buffer.
   */
  void push(const T* data)
  {
    await(room());
    unsigned int next = (m_put + 1) & MASK;
    m_buffer[next] = *data;
    m_put = next;
  }

  /**
   * Push given data in program memory into queue. Will yield until
   * room is available.
   * @param[in] data pointer to data buffer in program memory.
   */
  void push_P(const T* data)
  {
    await(room());
    unsigned int next = (m_put + 1) & MASK;
    memcpy_P(&m_buffer[next], data, sizeof(T));
    m_put = next;
  }

  /**
   * Pull data from queue to given buffer. Will yield until data is
   * available.
   * @param[in,out] data pointer to data buffer.
   */
  void pull(T* data)
  {
    await(available());
    unsigned int next = (m_get + 1) & MASK;
    m_get = next;
    *data = m_buffer[next];
  }

private:
  static const unsigned int MASK = (NMEMB - 1);
  volatile unsigned int m_put;
  volatile unsigned int m_get;
  T m_buffer[NMEMB];
};

#endif
