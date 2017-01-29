/**
 * @file SchedulerDemo.ino
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
 * This Arduino sketch shows how to use the Scheduler library.
 * Two tasks are scheduled together with the main task.
 * 1. The main task prints an iteration number and delays.
 * 2. The second task blinks the built-in LED and prints the
 *    current status.
 * 3. The third task reads a line from Serial input. The line is
 *    printed with number of yields during the input wait.
 */

#include <Scheduler.h>

// Check for SparkFun SAMD21 Breakout
#if defined(ARDUINO_ARCH_SAMD) && (USB_PID == 0x8D21)
#define Serial SerialUSB
#endif

void setup()
{
  Serial.begin(57600);
  while (!Serial);
  Serial.println(F("SchedulerDemo: started"));

  Serial.print(millis());
  Serial.println(F(":setup"));

  Scheduler.start(setup2, loop2);
  Scheduler.start(setup3, loop3);
}

void loop()
{
  // Main loop iteraction count
  static int i = 0;

  // Print main loop iterations
  Serial.print(millis());
  Serial.print(F(":loop::i="));
  Serial.println(i++);
  delay(500);

  Serial.print(millis());
  Serial.print(F(":loop::stack="));
  Serial.println(Scheduler.stack());
}

const int LED = 13;

void setup2()
{
  Serial.print(millis());
  Serial.println(F(":setup2"));
  pinMode(LED, OUTPUT);
}

void loop2()
{
  // Turn LED off
  Serial.print(millis());
  Serial.println(F(":loop2::led off"));
  digitalWrite(LED, LOW);
  delay(1000);

  // Turn LED on
  Serial.print(millis());
  Serial.println(F(":loop2::led on"));
  digitalWrite(LED, HIGH);
  delay(1000);

  Serial.print(millis());
  Serial.print(F(":loop2::stack="));
  Serial.println(Scheduler.stack());
}

void setup3()
{
  Serial.print(millis());
  Serial.println(F(":setup3"));
}

void loop3()
{
  static char* buf = NULL;

  // Check for buffer allocation
  if (buf == NULL) {
    buf = (char*) malloc(64);
    Serial.print(millis());
    Serial.print(F(":loop3:alloc:buf=0x"));
    Serial.println((int) buf, HEX);
    if (buf == NULL) {
      delay(1000);
      return;
    }
  }

  // Read line and yield while waiting for characters
  // Capture wait time and number of yields
  char* bp = buf;
  unsigned long yields = 0;
  unsigned long start = millis();
  int c;
  while ((c = Serial.read()) != '\n') {
    if (c > 0)
      *bp++ = c;
    else {
      yields += 1;
      yield();
    }
  }
  *bp = 0;

  // Print wait time, number of yields and line
  unsigned long stop = millis();
  unsigned long ms = stop - start;
  Serial.print(millis());
  Serial.print(F(":loop3:yields="));
  Serial.print(yields);
  Serial.print(F(",ms="));
  Serial.print(ms);
  Serial.print(F(",buf="));
  Serial.println(buf);

  // Check for buffer free command
  if (!strcmp_P(buf, (const char*) F("free"))) {
    Serial.print(millis());
    Serial.print(F(":loop3:free:buf=0x"));
    Serial.println((int) buf, HEX);
    free(buf);
    delay(500);
    buf = NULL;
  }

  Serial.print(millis());
  Serial.print(F(":loop3::stack="));
  Serial.println(Scheduler.stack());
}
