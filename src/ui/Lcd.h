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

#ifndef LCD_INC
#define LCD_INC

#include <Log.h>
#include <Misc.h>

#ifndef UNIT_TEST

#include <LiquidCrystal.h>

/**
* This class controls initialization and control of the LCD display (on Arduino)
*/
class Lcd {

private:
  LiquidCrystal *lcd;
  int updates;

public:
  Lcd(int rsPin, int enablePin, int d4Pin, int d5Pin, int d6Pin, int d7Pin);
  void initialize();
  void display(const char *upLine, const char *downLine);
};


#else

/**
* This class controls initialization and control of the LCD display (on PC).
*/
class Lcd {

private:

public:
  Lcd(int rsPin, int enablePin, int d4Pin, int d5Pin, int d6Pin, int d7Pin);
  void initialize();
  void display(const char *upLine, const char *downLine);
};

#endif // UNIT_TEST

#endif // LCD_INC
