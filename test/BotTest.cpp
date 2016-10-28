#ifdef UNIT_TEST
#ifdef BOT_TEST

// Auxiliary libraries
#include <unity.h>
#include <stdio.h>

// Library being tested
#include <Bot.h>
#include <Pump.h>

#define MODE_PRESSED true
#define SET_PRESSED true
#define BUTTON_NOT_PRESSED false

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
  int nroActors = 2;
  Pump pump0("PUMP0");
  Pump pump1("PUMP1");
  Actor *pumps[] = {&pump0, &pump1};
  Bot bot(displayLcdMockupFunctionString, pumps, nroActors);
  char buffer[16 + 1];

  TEST_ASSERT_EQUAL(WelcomeState, bot.getState());

  bot.cycle(BUTTON_NOT_PRESSED, BUTTON_NOT_PRESSED, false);
  TEST_ASSERT_EQUAL(WelcomeState, bot.getState());
  TEST_ASSERT_EQUAL_STRING("WELCOME!", *lcdContentUp);

  bot.cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigHourState, bot.getState());

  bot.cycle(false, false, false); // nothing pressed
  TEST_ASSERT_EQUAL(ConfigHourState, bot.getState());

  bot.cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigMinuteState, bot.getState());
  for (int l = 0; l < nroActors; l++) {
    bot.cycle(MODE_PRESSED, false, false);
    TEST_ASSERT_EQUAL(ConfigFrequenciesState, bot.getState());
  }

  bot.cycle(MODE_PRESSED, false, false); // done with the frequencies
  TEST_ASSERT_EQUAL(ConfigFrequenciesState, bot.getState());

  bot.cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigActorsState, bot.getState());
  TEST_ASSERT_EQUAL(0, bot.getAuxStateIndex());
  TEST_ASSERT_EQUAL(PumpConfigStateAmount, bot.getAuxSubstateIndex());

  bot.cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigActorsState, bot.getState());
  TEST_ASSERT_EQUAL(0, bot.getAuxStateIndex());
  TEST_ASSERT_EQUAL(PumpConfigStateAmount2, bot.getAuxSubstateIndex());

  bot.cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigActorsState, bot.getState());
  TEST_ASSERT_EQUAL(1, bot.getAuxStateIndex());
  TEST_ASSERT_EQUAL(PumpConfigStateAmount, bot.getAuxSubstateIndex());

  bot.cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigActorsState, bot.getState());
  TEST_ASSERT_EQUAL(1, bot.getAuxStateIndex());
  TEST_ASSERT_EQUAL(PumpConfigStateAmount2, bot.getAuxSubstateIndex());

  bot.cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigActorsState, bot.getState()); // done with actors

  bot.cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(RunState, bot.getState()); // new state
}

void test_bot_correctly_switches_infos(void) {
  int nroActors = 1;
  Pump pump0("PUMP0");
  Actor *pumps[] = {&pump0};
  Bot bot(displayLcdMockupFunctionString, pumps, nroActors);

  bot.setState(RunState);

  TEST_ASSERT_EQUAL(nroActors - 1, bot.getAuxStateIndex()); // pump actor
  TEST_ASSERT_EQUAL(0, bot.getAuxSubstateIndex()); // first pump info state (unique of the pump itself)

  bot.cycle(false, SET_PRESSED, false);

  TEST_ASSERT_EQUAL(nroActors - 1, bot.getAuxStateIndex()); // pump actor
  TEST_ASSERT_EQUAL(1, bot.getAuxSubstateIndex()); // second pump info state (frequency)

  bot.cycle(false, SET_PRESSED, false);

  TEST_ASSERT_EQUAL(nroActors, bot.getAuxStateIndex()); // general info
  TEST_ASSERT_EQUAL(0, bot.getAuxSubstateIndex()); // clock

  bot.cycle(false, SET_PRESSED, false);

  TEST_ASSERT_EQUAL(nroActors - 1, bot.getAuxStateIndex());
  TEST_ASSERT_EQUAL(0, bot.getAuxSubstateIndex());

}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_bot_correctly_switches_states);
  RUN_TEST(test_bot_correctly_switches_infos);
  UNITY_END();
}

#endif // BOT_TEST
#endif // UNIT_TEST
