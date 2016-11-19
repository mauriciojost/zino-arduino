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
  int nroActors = 1;
  int nroConfigurables = nroActors + 1;

  int indexConfigurable0 = 0;
  Clock clock(nroActors);
  int indexConfigurable1 = 1;
  TestActor a0("ACT0");

  Actor *dumbActors[] = {&a0};
  Configurable *configurables[] = {&a0, &clock};

  Bot* bot = new Bot(&clock, dumbActors, nroActors, configurables, nroConfigurables);
  bot->setStdoutFunction(displayLcdMockupFunctionString);

  bot->cycle(false, false, false); // nothing pressed

  // WELCOME MODE
  TEST_ASSERT_EQUAL(WelcomeMode, bot->getMode());
  TEST_ASSERT_EQUAL_STRING(MSG_BOT_STATE_WELCOME, *lcdContentUp);

  // HELP MODE
  bot->cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(HelpMode, bot->getMode());

  bot->cycle(false, false, false); // nothing pressed
  TEST_ASSERT_EQUAL(HelpMode, bot->getMode());

 // CONFIG CONFIGURABLES
  bot->cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigActorsMode, bot->getMode());
  TEST_ASSERT_EQUAL(indexConfigurable0, bot->getConfigurableIndex()); // first configurable (actor)
  TEST_ASSERT_EQUAL(TestActorConfigStateAmount, bot->getConfigurableStateIndex()); // first configuration state

  bot->cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigActorsMode, bot->getMode());
  TEST_ASSERT_EQUAL(indexConfigurable0, bot->getConfigurableIndex());
  TEST_ASSERT_EQUAL(TestActorConfigStateAmount2, bot->getConfigurableStateIndex()); // second configuration state

  bot->cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigActorsMode, bot->getMode()); // second configurable (clock)
  TEST_ASSERT_EQUAL(indexConfigurable1, bot->getConfigurableIndex());
  TEST_ASSERT_EQUAL(0, bot->getConfigurableStateIndex());

  bot->cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigActorsMode, bot->getMode());
  TEST_ASSERT_EQUAL(indexConfigurable1, bot->getConfigurableIndex());
  TEST_ASSERT_EQUAL(1, bot->getConfigurableStateIndex());

  bot->cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigActorsMode, bot->getMode());
  TEST_ASSERT_EQUAL(indexConfigurable1, bot->getConfigurableIndex());
  TEST_ASSERT_EQUAL(2, bot->getConfigurableStateIndex());

  bot->cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigActorsMode, bot->getMode());
  TEST_ASSERT_EQUAL(indexConfigurable1, bot->getConfigurableIndex());
  TEST_ASSERT_EQUAL(3, bot->getConfigurableStateIndex());

  bot->cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigActorsMode, bot->getMode()); // done with actors

 // FREQUENCIES CONFIGURATION STATE
  bot->cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigFrequenciesMode, bot->getMode());
  TEST_ASSERT_EQUAL(0, bot->getConfigurableIndex()); // first actor

  bot->cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigFrequenciesMode, bot->getMode());
  TEST_ASSERT_EQUAL(0, bot->getConfigurableIndex()); // done with actors

 // RUN STATE
  bot->cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(RunMode, bot->getMode());

  // CONFIG CONFIGURABLES STATE
  bot->cycle(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigActorsMode, bot->getMode());

  // and so on...

}

void test_bot_correctly_switches_infos(void) {
  int nroActors = 1;
  int nroConfigurables = 1;
  TestActor a0("ACTOR0");
  Clock clock(nroActors);
  Actor *dumbActors[] = {&a0};
  Configurable *configurables[] = {&a0}; // TODO: include clock
  Bot* bot = new Bot(&clock, dumbActors, nroActors, configurables, nroConfigurables);
  bot->setStdoutFunction(displayLcdMockupFunctionString);

  bot->setMode(RunMode);

  TEST_ASSERT_EQUAL(nroActors - 1, bot->getConfigurableIndex()); // dumbActor actor
  TEST_ASSERT_EQUAL(0, bot->getConfigurableStateIndex());        // first dumbActor info state (unique of the dumbActor itself)

  bot->cycle(false, SET_PRESSED, false);

  TEST_ASSERT_EQUAL(nroActors - 1, bot->getConfigurableIndex()); // dumbActor actor
  TEST_ASSERT_EQUAL(1, bot->getConfigurableStateIndex());        // second dumbActor info state (last watering time)

  bot->cycle(false, SET_PRESSED, false);

  TEST_ASSERT_EQUAL(0, bot->getConfigurableIndex());            // back to the origin
  TEST_ASSERT_EQUAL(0, bot->getConfigurableStateIndex());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_bot_correctly_switches_modes);
  RUN_TEST(test_bot_correctly_switches_infos);
  UNITY_END();
}

#endif // BOT_TEST
#endif // UNIT_TEST
