#ifndef UNIT_TEST

#ifndef LCD_INC
#define LCD_INC

#include <Log.h>
#include <Misc.h>
#include <LiquidCrystal.h>

/**
* This class controls initialization and control of the LCD display.
*/
class Lcd {

private:
  LiquidCrystal* lcd;
  int updates;

public:
  Lcd(int rsPin, int enablePin, int d4Pin, int d5Pin, int d6Pin, int d7Pin);
  void initialize();
  void display(const char *upLine, const char *downLine);

};

#endif // LCD_INC

#endif // UNIT_TEST
