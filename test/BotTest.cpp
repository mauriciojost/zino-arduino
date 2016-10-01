// Auxiliary libraries
#include <unity.h>
#include <stdio.h>

// Library being tested
#include <Bot.h>

#define MODE_PRESSED true
#define SET_PRESSED true
#define WDT_INTERRUPT true

#ifdef UNIT_TEST

const char* EMPTY_MSG = "";

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

  printf("----------------\n");
  printf("%s\n", *lcdContentUp);
  printf("%s\n", *lcdContentDown);
  printf("----------------\n");
}

void test_bot_correctly_switches_states(void) {

  Bot bot(displayLcdMockupFunctionString);

  TEST_ASSERT_EQUAL(bot.state, WelcomeState);

  bot.run(false, false, false);
  TEST_ASSERT_EQUAL(bot.state, WelcomeState);
  TEST_ASSERT_EQUAL_STRING(*lcdContentUp, "WELCOME");

  bot.run(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(bot.state, ConfigPeriodState);
  TEST_ASSERT_EQUAL_STRING(*lcdContentUp, "PERIOD");

  bot.run(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(bot.state, ConfigAmountState);
  TEST_ASSERT_EQUAL_STRING(*lcdContentUp, "AMOUNT");

}

int main() {
  UNITY_BEGIN(); // IMPORTANT LINE!
  RUN_TEST(test_bot_correctly_switches_states);
  UNITY_END(); // stop unit testing
}

#endif
