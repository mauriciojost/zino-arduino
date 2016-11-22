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
