#ifndef UNIT_TEST

#ifndef LCD_INC
#define LCD_INC

#include <Log.h>
#include <Misc.h>
#include <LiquidCrystal.h>

class Lcd {

private:
  LiquidCrystal* lcd;

public:
  Lcd(int rsPin, int enablePin, int d4Pin, int d5Pin, int d6Pin, int d7Pin);
  void initialize();
  void display(const char *str1, const char *str2, int cycles);

};

#endif // LCD_INC

#endif // UNIT_TEST
