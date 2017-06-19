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
  this->lastPosition = -1;
}

void Servox::controlServo(bool active, int position, int delayMs, bool smooth) {
  if (active) {
    servo->attach(pin);
    servo->write(position);
    delay(delayMs); // let servo be controlled
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
