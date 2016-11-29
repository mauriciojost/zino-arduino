#ifdef UNIT_TEST
#ifdef MODULE_TEST

// Auxiliary libraries
#include <unity.h>
#include <stdio.h>
#include <string.h>

// Library being tested
#include <Bot.h>
#include <Module.h>
#include <actors/TestActor.h>
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

int readLevel() {
  return 0;
}

void digitalWriteMocked(unsigned char pin, unsigned char value) {}

void test_module_uses_lcd_correctly(void) {
  Module m;

  m.setup();
  m.setFactor(SECS_CYCLE_FACTOR_DEFAULT);
  m.setStdoutWriteFunction(displayLcdMockupFunctionString);
  m.setReadLevelFunction(readLevel);
  m.setDigitalWriteFunction(digitalWriteMocked);

  for (int mode = 0; mode < 20; mode++) {
    for (int set = 0; set < 2; set++) {
      m.loop(false, SET_PRESSED, false);
      m.loop(false, false, WDT_INTERRUPT);
    }
    m.loop(MODE_PRESSED, false, false);
  }
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_module_uses_lcd_correctly);
  UNITY_END();
}

#endif // MODULE_TEST
#endif // UNIT_TEST