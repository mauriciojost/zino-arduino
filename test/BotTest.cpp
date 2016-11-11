#ifdef UNIT_TEST
#ifdef BOT_TEST

// Auxiliary libraries
#include <unity.h>
#include <stdio.h>

// Library being tested
#include <Bot.h>
#include <actors/TestActor.h>

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

  printf("\nLCD:\n");
  printf("%s----------------%s\n", KWHTBLU, KNRM);
  printf("%s%s%s\n", KBLU, *lcdContentUp, KNRM);
  printf("%s%s%s\n", KBLU, *lcdContentDown, KNRM);
  printf("%s----------------%s\n\n\n", KWHTBLU, KNRM);
}

void test_bot_correctly_switches_modes(void) {
  int nroActors = 2;
  int indexActor0 = 0;
  int indexActor1 = 1;
  TestActor a0("ACT0");
  TestActor a1("ACT1");
  Actor *dumbActors[] = {&a0, &a1};
  Bot bot(displayLcdMockupFunctionString, dumbActors, nroActors);
  char buffer[16 + 1];

  TEST_ASSERT_EQUAL(WelcomeMode, bot.getMode());

  bot.cycle(BUTTON_NOT_PRESSED, BUTTON_NOT_PRESSED, false);
  TEST_ASSERT_EQUAL(WelcomeMode, bot.getMode()); // WELCOME
  TEST_ASSERT_EQUAL_STRING(MSG_BOT_STATE_WELCOME, *lcdContentUp);

  bot.cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigHourMode, bot.getMode()); // CONFIG HOUR

  bot.cycle(false, false, false); // nothing pressed
  TEST_ASSERT_EQUAL(ConfigHourMode, bot.getMode());

  bot.cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigMinuteMode, bot.getMode()); // CONFIG MINUTE

  bot.cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigFactorMode, bot.getMode()); // CONFIG FACTOR

  bot.cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigActorsMode, bot.getMode()); // CONFIG ACTORS
  TEST_ASSERT_EQUAL(indexActor0, bot.getAuxStateIndex());
  TEST_ASSERT_EQUAL(TestActorConfigStateAmount, bot.getAuxSubstateIndex());

  bot.cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigActorsMode, bot.getMode());
  TEST_ASSERT_EQUAL(indexActor0, bot.getAuxStateIndex());
  TEST_ASSERT_EQUAL(TestActorConfigStateAmount2, bot.getAuxSubstateIndex());

  bot.cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigActorsMode, bot.getMode());
  TEST_ASSERT_EQUAL(indexActor0, bot.getAuxStateIndex());
  TEST_ASSERT_EQUAL(TestActorConfigStateDelimiter, bot.getAuxSubstateIndex()); // frequency configuration

  bot.cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigActorsMode, bot.getMode());
  TEST_ASSERT_EQUAL(indexActor1, bot.getAuxStateIndex());
  TEST_ASSERT_EQUAL(TestActorConfigStateAmount, bot.getAuxSubstateIndex());

  bot.cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigActorsMode, bot.getMode());
  TEST_ASSERT_EQUAL(indexActor1, bot.getAuxStateIndex());
  TEST_ASSERT_EQUAL(TestActorConfigStateAmount2, bot.getAuxSubstateIndex());

  bot.cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigActorsMode, bot.getMode());
  TEST_ASSERT_EQUAL(indexActor1, bot.getAuxStateIndex());
  TEST_ASSERT_EQUAL(TestActorConfigStateDelimiter, bot.getAuxSubstateIndex()); // frequency configuration

  bot.cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigActorsMode, bot.getMode()); // done with actors

  bot.cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(RunMode, bot.getMode()); // RUN STATE
}

void test_bot_correctly_switches_infos(void) {
  int nroActors = 1;
  TestActor a0("ACTOR0");
  Actor *dumbActors[] = {&a0};
  Bot bot(displayLcdMockupFunctionString, dumbActors, nroActors);

  bot.setMode(RunMode);

  TEST_ASSERT_EQUAL(nroActors - 1, bot.getAuxStateIndex()); // dumbActor actor
  TEST_ASSERT_EQUAL(0, bot.getAuxSubstateIndex());          // first dumbActor info state
                                                            // (unique of the dumbActor
                                                            // itself)

  bot.cycle(false, SET_PRESSED, false);

  TEST_ASSERT_EQUAL(nroActors - 1, bot.getAuxStateIndex()); // dumbActor actor
  TEST_ASSERT_EQUAL(1, bot.getAuxSubstateIndex());          // second dumbActor info
                                                            // state (last watering time)

  bot.cycle(false, SET_PRESSED, false);

  TEST_ASSERT_EQUAL(nroActors - 1, bot.getAuxStateIndex()); // dumbActor actor
  TEST_ASSERT_EQUAL(2, bot.getAuxSubstateIndex());          // second dumbActor info state (frequency)

  bot.cycle(false, SET_PRESSED, false);

  TEST_ASSERT_EQUAL(nroActors, bot.getAuxStateIndex()); // general info
  TEST_ASSERT_EQUAL(0, bot.getAuxSubstateIndex());      // clock

  bot.cycle(false, SET_PRESSED, false);

  TEST_ASSERT_EQUAL(nroActors - 1, bot.getAuxStateIndex());
  TEST_ASSERT_EQUAL(0, bot.getAuxSubstateIndex());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_bot_correctly_switches_modes);
  RUN_TEST(test_bot_correctly_switches_infos);
  UNITY_END();
}

#endif // BOT_TEST
#endif // UNIT_TEST
