#ifndef UNIT_TEST

#include <ui/Lcd.h>
#include <ui/Messages.h>

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

#endif // UNIT_TEST
