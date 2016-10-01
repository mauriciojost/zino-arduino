// Auxiliary libraries
#include <unity.h>
#include <stdio.h>

// Library being tested
#include <Bot.h>

#define BUTTON0_PRESSED true
#define BUTTON1_PRESSED true
#define WDT_INTERRUPT true

#ifdef UNIT_TEST

const char **lcdContentUp = &lcdMessageWelcomeUp;
const char **lcdContentDown = &lcdMessageWelcomeDo;

void setUp(void) {
  lcdContentUp = &lcdMessageWelcomeUp;
  lcdContentDown = &lcdMessageWelcomeDo;
}

void tearDown(void) {}

void displayLcdMockupFunctionString(const char *str1, const char *str2) {

  printf("\n\n\nCALLED\n");

  printf("const welcnup: %X\n", lcdMessageWelcomeUp);
  printf("const welcndo: %X\n", lcdMessageWelcomeDo);
  printf("const rununup: %X\n", lcdMessageRunUp);
  printf("const runundo: %X\n", lcdMessageRunDo);
  printf("const confiup: %X\n", lcdMessageConfigUp);
  printf("const confido: %X\n", lcdMessageConfigDo);
  printf("const null   : %X\n", lcdMessageNull);

  printf("LCD ----------\n");
  printf("LCD up    : %X\n", *lcdContentUp);
  printf("LCD       : %X\n", *lcdContentDown);
  printf("LCD ----------\n");

  lcdContentUp = &str1;
  lcdContentDown = &str2;

  printf("LCD ----------\n");
  printf("LCD up    : %X\n", *lcdContentUp);
  printf("LCD       : %X\n", *lcdContentDown);
  printf("LCD ----------\n");

}

void test_bot_correctly_switches_states(void) {
  Bot bot(displayLcdMockupFunctionString);

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
