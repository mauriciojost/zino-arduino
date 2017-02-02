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

#include <ui/Lcd.h>
#include <ui/Messages.h>

#ifndef UNIT_TEST

byte modeButtonIcon[8] = {
    B11111,
    B11011,
    B11101,
    B00000,
    B11101,
    B11011,
    B11111,
};

byte setButtonIcon[8] = {
    B11111,
    B11011,
    B10001,
    B11011,
    B11111,
    B10001,
    B11111,
};

byte waterDropIcon[8] = {
    B00000,
    B00100,
    B00100,
    B01110,
    B11111,
    B11111,
    B01110,
};

Lcd::Lcd(int rsPin, int enablePin, int d4Pin, int d5Pin, int d6Pin, int d7Pin) {
  lcd = new LiquidCrystal(rsPin, enablePin, d4Pin, d5Pin, d6Pin, d7Pin);
}

void Lcd::initialize() {
  lcd->begin(16, 2);
  lcd->noAutoscroll();
  lcd->leftToRight();
  lcd->noBlink();
  lcd->createChar(1, modeButtonIcon); // will be printed whenever character \1 is used
  lcd->createChar(2, setButtonIcon);  // will be printed whenever character \2 is used
  lcd->createChar(3, waterDropIcon);  // will be printed whenever character \3 is used
  lcd->clear();
}

void Lcd::display(const char *str1, const char *str2) {
  char bufferUp[LCD_LENGTH + 1];
  char bufferDown[LCD_LENGTH + 1];
  updates++;
  bool onceInAWhile = ((updates % 20) == 0);
  if (onceInAWhile) {
    initialize(); // did not find a way a better way to ensure LCD won't get
                  // corrupt due to load noise
  }
  sprintf(bufferUp, "%-16.16s", str1);
  sprintf(bufferDown, "%-16.16s", str2);

  lcd->setCursor(0, 0);
  lcd->print(bufferUp);
  lcd->setCursor(0, 1);
  lcd->print(bufferDown);
}

#else

Lcd::Lcd(int rsPin, int enablePin, int d4Pin, int d5Pin, int d6Pin, int d7Pin) {}

void Lcd::initialize() {}

void Lcd::display(const char *str1, const char *str2) {
  printf("\nLCD:\n");
  printf("%s----------------%s\n", KWHTBLU, KNRM);
  printf("%s%s%s\n", KBLU, str1, KNRM);
  printf("%s%s%s\n", KBLU, str2, KNRM);
  printf("%s----------------%s\n\n\n", KWHTBLU, KNRM);
}

#endif // UNIT_TEST
