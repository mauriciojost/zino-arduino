#ifdef UNIT_TEST
#ifdef BOT_TEST

// Auxiliary libraries
#include <unity.h>
#include <stdio.h>

// Library being tested
#include <Bot.h>

#define MODE_PRESSED true
#define SET_PRESSED true
#define BUTTON_NOT_PRESSED false
#define WDT_INTERRUPT true
#define TIME_GOES_ON true

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

  printf("\nLCD:\n----------------\n");
  printf("%s\n", *lcdContentUp);
  printf("%s\n", *lcdContentDown);
  printf("----------------\n\n\n");
}

void test_bot_correctly_switches_states(void) {
  Pump pump0("PUMP0");
  Pump pump1("PUMP1");
  Pump* pumps[] = {&pump0, &pump1};
  Bot bot(displayLcdMockupFunctionString, pumps, 2);
  char buffer[16 + 1];

  TEST_ASSERT_EQUAL(WelcomeState, bot.state);

  bot.cycle(BUTTON_NOT_PRESSED, BUTTON_NOT_PRESSED, false);
  TEST_ASSERT_EQUAL(WelcomeState, bot.state);
  TEST_ASSERT_EQUAL_STRING("WELCOME!", *lcdContentUp);

  bot.cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigPeriodState, bot.state);

  bot.cycle(false, false, false); // nothing pressed
  TEST_ASSERT_EQUAL(ConfigPeriodState, bot.state);

  bot.cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigActorsState, bot.state);
  TEST_ASSERT_EQUAL(0, bot.actorIndex);
  TEST_ASSERT_EQUAL(PumpConfigAmountState, pump0.currentConfigState(buffer));

  bot.cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigActorsState, bot.state);
  TEST_ASSERT_EQUAL(0, bot.actorIndex);
  TEST_ASSERT_EQUAL(PumpConfigAmountState2, pump0.currentConfigState(buffer));

  bot.cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigActorsState, bot.state); // done with pump0

  bot.cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigActorsState, bot.state);
  TEST_ASSERT_EQUAL(1, bot.actorIndex);
  TEST_ASSERT_EQUAL(PumpConfigAmountState, pump1.currentConfigState(buffer));

  bot.cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigActorsState, bot.state);
  TEST_ASSERT_EQUAL(1, bot.actorIndex);
  TEST_ASSERT_EQUAL(PumpConfigAmountState2, pump1.currentConfigState(buffer));

  bot.cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigActorsState, bot.state); // done with pump1

  bot.cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigActorsState, bot.state); // done with actors

  bot.cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigHourState, bot.state); // new state

}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_bot_correctly_switches_states);
  UNITY_END();
}

#endif // BOT_TEST
#endif // UNIT_TEST
