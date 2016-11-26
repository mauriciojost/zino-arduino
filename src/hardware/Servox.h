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

#ifndef SERVOX_INC
#define SERVOX_INC

#include <Log.h>
#include <Misc.h>

#ifndef UNIT_TEST
#include <Servo.h>
#endif // UNIT_TEST

/**
* This class controls initialization and control of the servo.
*/
class Servox {

private:
#ifndef UNIT_TEST
  Servo *servo;
  unsigned char pin;
#endif // UNIT_TEST

public:
  Servox(unsigned char servoPin);
  void controlServo(bool active, int position);
};

#endif // SERVOX_INC
