/**
 * @file Scheduler/Channel.h
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

#ifndef SCHEDULER_CHANNEL_H
#define SCHEDULER_CHANNEL_H

#include <Scheduler.h>

class Channel {
public:
  /**
   * Initiate channel for message passing between tasks.
   */
 Channel() :
   m_buf(NULL),
   m_max(0),
   m_size(0),
   m_ticket(0),
   m_serving(0)
  {}

  /**
   * Send given message, buffer and size, to receiving task. Wait
   * until receiver task is ready before copying message to receiver
   * buffer. Multiple senders will queue in order. Returns message
   * size if successful otherwise negative error code.
   * @param[in] buf message buffer.
   * @param[in] size message size.
   * @return message size or negative error code.
   */
  int send(const void* buf, size_t size)
  {
    // Take a ticket and wait for service
    uint8_t ticket = m_ticket++;
    await(ticket == m_serving && m_buf != NULL);

    // Check that the receiver buffer can hold the message
    if (size > m_max) {
      m_serving += 1;
      return (-1);
    }

    // Copy message to receiver buffer
    memcpy(m_buf, buf, size);
    m_size = size;
    return (size);
  }

  /**
   * Receive message from channel. Wait for sender task to copy
   * message into given receive buffer. Returns received message size.
   * @param[in] buf message buffer.
   * @param[in] size message size.
   * @return received message size.
   */
  int recv(void* buf, size_t size)
  {
    // Wait for the channel to be free for receive
    await(m_buf == NULL);
    m_buf = buf;
    m_max = size;
    m_size = 0;

    // Wait for message from sender
    await(m_size != 0);
    int res = m_size;
    m_buf = NULL;
    m_max = 0;
    m_size = 0;
    m_serving += 1;

    // Return size of received message
    return (res);
  }

protected:
  void* m_buf;			//!< Message buffer
  volatile size_t m_max;	//!< Max message size
  volatile size_t m_size;	//!< Actual message size
  volatile uint8_t m_ticket;	//!< Sender ticket
  volatile uint8_t m_serving;	//!< Next sender ticket
};

#endif
