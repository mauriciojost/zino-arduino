// Auxiliary libraries
#include <unity.h>
#include <iostream>
#include <string>
#include <stdio.h>

// Library being tested
#include <Bot.h>

#define BUTTON0_PRESSED true
#define BUTTON1_PRESSED true
#define WDT_INTERRUPT true

#ifdef UNIT_TEST

void setUp(void) {}

void tearDown(void) {}

int lcdCursorY = 0;
const char **lcdContentUp = NULL;
const char **lcdContentDown = NULL;

void displayLcdMockupFunctionString(const char *str) {
  if (lcdCursorY == 0) {
    printf("+----------------+\n");
    printf("|%s|\n", str);
    lcdContentUp = &str;
  } else {
    printf("|%s|\n", str);
    printf("+----------------+\n");
    lcdContentDown = &str;
  }
}

void displayLcdMockupFunctionInt(int i) {
  // not used
}

void setCursorLcdMockupFunction(int x, int y) {
  lcdCursorY = y;
}

void test_bot_correctly_switches_states(void) {
  Bot bot(displayLcdMockupFunctionString, displayLcdMockupFunctionInt, setCursorLcdMockupFunction);

  TEST_ASSERT_EQUAL(bot.state, WelcomeState); // welcome state
  bot.run(false, false, false);
  TEST_ASSERT_EQUAL(bot.state, WelcomeState); // keep in the same state
  bot.run(BUTTON0_PRESSED, false, false);
  TEST_ASSERT_EQUAL(bot.state, RunState);             // to run
  TEST_ASSERT_EQUAL_STRING(*lcdContentUp, lcdMessageRunUp); // the LCD should always follow
  bot.run(false, BUTTON1_PRESSED, false);
  TEST_ASSERT_EQUAL(bot.state, WelcomeState); // back to the welcome state
  TEST_ASSERT_EQUAL_STRING(*lcdContentUp, lcdMessageWelcomeUp); // the LCD should always follow
  bot.run(false, BUTTON1_PRESSED, false);
  TEST_ASSERT_EQUAL(bot.state, ConfigState); // to the config state
  TEST_ASSERT_EQUAL_STRING(*lcdContentUp, lcdMessageConfigUp); // the LCD should always follow
}

int main() {
  UNITY_BEGIN(); // IMPORTANT LINE!
  RUN_TEST(test_bot_correctly_switches_states);
  UNITY_END(); // stop unit testing
}

#endif
