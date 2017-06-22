#ifdef UNIT_TEST

// Auxiliary libraries
#include <unity.h>
#include <stdio.h>
#include <string.h>

// Library being tested
#include <main4ino/Bot.h>
#include <Module.h>
#include <main4ino/TestActor.h>
#include <actors/Level.h>
#include <actors/Pump.h>
#include <actors/Delayer.h>

#define MODE_PRESSED true
#define SET_PRESSED true
#define WDT_INTERRUPT true

const char *EMPTY_MSG = "";

const char **lcdContentUp = &EMPTY_MSG;
const char **lcdContentDown = &EMPTY_MSG;

void setUp(void) {
  lcdContentUp = &EMPTY_MSG;
  lcdContentDown = &EMPTY_MSG;
}

void tearDown(void) {}

unsigned char eepromRead(int address) {
  return 20;
}

void eepromSave(int address, unsigned char byte) {
  printf("EEPROM w[%x] = %x\n", address, byte);
}

void displayLcdMockupFunctionString(const char *str1, const char *str2) {

  lcdContentUp = &str1;
  lcdContentDown = &str2;

  printf("\nLCD:\n");
  printf("%s----------------%s\n", KWHTBLU, KNRM);
  printf("%s%s%s\n", KBLU, *lcdContentUp, KNRM);
  printf("%s%s%s\n", KBLU, *lcdContentDown, KNRM);
  printf("%s----------------%s\n\n\n", KWHTBLU, KNRM);

  TEST_ASSERT(strlen(str1) <= LCD_LENGTH);
  TEST_ASSERT(strlen(str2) <= LCD_LENGTH);
}

void controlServoFunction(int v, int ms, bool pump, bool smooth) {
  printf("SERVO: %d %d %d %d\n", v, ms, pump, smooth);
}

void displayLcdMockupFunctionStringMute(const char *str1, const char *str2) {

  lcdContentUp = &str1;
  lcdContentDown = &str2;

  bool goodLenghtContentUp = (strlen(str1) <= LCD_LENGTH);
  bool goodLenghtContentDo = (strlen(str2) <= LCD_LENGTH);

  if (!goodLenghtContentUp || !goodLenghtContentDo) {
    printf("UP: %s\n", *lcdContentUp);
    printf("DO: %s\n", *lcdContentDown);
  }

  TEST_ASSERT(goodLenghtContentUp);
  TEST_ASSERT(goodLenghtContentDo);
}

int readLevel() {
  return 0;
}

void digitalWriteMocked(unsigned char pin, unsigned char value) {}

void test_module_uses_lcd_correctly(void) {
  Module m;
  m.setEepromRead(eepromRead);
  m.setEepromSave(eepromSave);
  m.setServoWriteFunction(controlServoFunction);
  m.setup();
  m.setFactor(SECS_CYCLE_FACTOR_DEFAULT);
  m.setStdoutWriteFunction(displayLcdMockupFunctionStringMute);
  m.setReadLevelFunction(readLevel);
  m.setDigitalWriteFunction(digitalWriteMocked);

  for (int mode = 0; mode < 20; mode++) {
    for (int set = 0; set < 2; set++) {
      m.loop(false, SET_PRESSED, false);
      m.loop(false, false, true);
      m.loop(false, false, true);
    }
    m.loop(MODE_PRESSED, false, true);
  }
}

void test_module_show_screens(void) {
  Module m;
  m.setEepromRead(eepromRead);
  m.setEepromSave(eepromSave);
  m.setServoWriteFunction(controlServoFunction);
  m.setup();
  m.setFactor(SECS_CYCLE_FACTOR_DEFAULT);
  m.setStdoutWriteFunction(displayLcdMockupFunctionString);
  m.setReadLevelFunction(readLevel);
  m.setDigitalWriteFunction(digitalWriteMocked);

  for (int mode = 0; mode < 20; mode++) {
    for (int set = 0; set < 2; set++) {
      m.loop(false, SET_PRESSED, false);
    }
    m.loop(MODE_PRESSED, false, true);
  }
}

#endif // UNIT_TEST
