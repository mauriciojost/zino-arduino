/*
 * ZINO: low-budget autonomous plant care system.
 *
 * Copyright 2016 Mauricio Jost
 * Contact: mauriciojostx@gmail.com
 *
 * This file is part of ZINO.
 *
 * ZINO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * ZINO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ZINO. If not, see <http://www.gnu.org/licenses/>.
 */

#include <hardware/Servox.h>

#ifndef UNIT_TEST
#define CLASS "Servox"

Servox::Servox(unsigned char servoPin) {
  this->servo = new Servo();
  this->pin = servoPin;
  this->lastPosition = 90;
}

void Servox::controlServo(bool active, int position, int delayMs, bool smooth) {
  if (active) {
    servo->attach(pin);
    if (smooth) {
      int steps = 5;
      if (delayMs > 2000) {steps = 20;}
      int stepMs = delayMs / steps;
      float delta = position - lastPosition;
      for (int i=0; i < steps - 1; i++) {
        int cuPos = lastPosition + ((delta * i) / steps);
        servo->write(cuPos);
        delay(stepMs);
      }
      servo->write(position);
      delay(stepMs);
    } else {
      servo->write(position);
      delay(delayMs);
    }
    servo->detach();
    lastPosition = position;
  }
}

#else  //  UNIT_TEST

Servox::Servox(unsigned char servoPin) {}
void Servox::controlServo(bool active, int position, int delayMs, bool smooth) {
  if (active) {
    printf("\nSERVO: ON %d\n", position);
  } else {
    printf("\nSERVO: OFF\n");
  }
}
#endif // UNIT_TEST

int Servox::getLastPosition() {
  return lastPosition;
}
