/*
 * ZINO: low-budget autonomous plant care system.
 *
 * Copyright 2016 Mauricio Jost
 * Contact: mauriciojostx@gmail.com
 *
 * This file is part of ZINO.
 *
 * ZINO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * ZINO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ZINO. If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef UNIT_TEST
#ifdef BOT_TEST

// Auxiliary libraries
#include <unity.h>
#include <stdio.h>
#include <string.h>

// Library being tested
#include <Bot.h>
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

void test_bot_correctly_switches_modes(void) {
  int nroActors = 1;
  int nroConfigurables = nroActors + 1;

  int indexConfigurable0 = 0;
  Clock clock(nroActors);
  int indexConfigurable1 = 1;
  TestActor a0("ACT0", true);

  Actor *dumbActors[] = {&a0, 0};
  Configurable *configurables[] = {&a0, &clock, 0};

  Bot *bot = new Bot(&clock, dumbActors, configurables);
  bot->setStdoutFunction(displayLcdMockupFunctionString);

  bot->cycle(false, false, TimingInterruptCycle, 0.0f); // nothing pressed

  // WELCOME MODE
  TEST_ASSERT_EQUAL(WelcomeMode, bot->getMode());
  TEST_ASSERT_EQUAL_STRING(MSG_BOT_STATE_WELCOME, *lcdContentUp);

  // HELP MODE
  bot->cycle(MODE_PRESSED, false, TimingInterruptNone, 0.0f);
  TEST_ASSERT_EQUAL(HelpMode, bot->getMode());

  bot->cycle(false, false, TimingInterruptCycle, 0.0f); // nothing pressed
  TEST_ASSERT_EQUAL(HelpMode, bot->getMode());

  // CONFIG CONFIGURABLES
  bot->cycle(MODE_PRESSED, false, TimingInterruptNone, 0.0f);
  TEST_ASSERT_EQUAL(ConfigConfigurablesMode, bot->getMode());
  TEST_ASSERT_EQUAL(indexConfigurable0, bot->getConfigurableIndex());              // first configurable (actor)
  TEST_ASSERT_EQUAL(TestActorConfigStateAmount, bot->getConfigurableStateIndex()); // first configuration state

  bot->cycle(MODE_PRESSED, false, TimingInterruptNone, 0.0f);
  TEST_ASSERT_EQUAL(ConfigConfigurablesMode, bot->getMode());
  TEST_ASSERT_EQUAL(indexConfigurable0, bot->getConfigurableIndex());
  TEST_ASSERT_EQUAL(TestActorConfigStateAmount2, bot->getConfigurableStateIndex()); // second configuration state

  bot->cycle(MODE_PRESSED, false, TimingInterruptNone, 0.0f);
  TEST_ASSERT_EQUAL(ConfigConfigurablesMode, bot->getMode()); // second configurable (clock)
  TEST_ASSERT_EQUAL(indexConfigurable1, bot->getConfigurableIndex());
  TEST_ASSERT_EQUAL(ClockConfigStateHours, bot->getConfigurableStateIndex());

  bot->cycle(false, false, TimingInterruptCycle, 0.0f); // these are not supposed to affect at all mode changes
  bot->cycle(false, false, TimingInterruptSubCycle, 0.1f);
  bot->cycle(false, false, TimingInterruptSubCycle, 0.2f);

  bot->cycle(MODE_PRESSED, false, TimingInterruptNone, 0.0f);
  TEST_ASSERT_EQUAL(ConfigConfigurablesMode, bot->getMode());
  TEST_ASSERT_EQUAL(indexConfigurable1, bot->getConfigurableIndex());
  TEST_ASSERT_EQUAL(ClockConfigStateMinutes, bot->getConfigurableStateIndex());

  bot->cycle(MODE_PRESSED, false, TimingInterruptNone, 0.0f);
  TEST_ASSERT_EQUAL(ConfigConfigurablesMode, bot->getMode());
  TEST_ASSERT_EQUAL(indexConfigurable1, bot->getConfigurableIndex());
  TEST_ASSERT_EQUAL(ClockConfigStateAdvanced, bot->getConfigurableStateIndex());

  bot->cycle(false, false, TimingInterruptCycle, 0.0f); // these are not supposed to affect at all mode changes
  bot->cycle(false, false, TimingInterruptSubCycle, 0.1f);
  bot->cycle(false, false, TimingInterruptSubCycle, 0.2f);

  bot->cycle(MODE_PRESSED, false, TimingInterruptNone, 0.0f);
  TEST_ASSERT_EQUAL(ConfigConfigurablesMode, bot->getMode()); // done with actors

  // FREQUENCIES CONFIGURATION STATE
  bot->cycle(MODE_PRESSED, false, TimingInterruptNone, 0.0f);
  TEST_ASSERT_EQUAL(ConfigActorFrequenciesMode, bot->getMode());
  TEST_ASSERT_EQUAL(0, bot->getConfigurableIndex()); // first actor

  bot->cycle(MODE_PRESSED, false, TimingInterruptNone, 0.0f);
  TEST_ASSERT_EQUAL(ConfigActorFrequenciesMode, bot->getMode());
  TEST_ASSERT_EQUAL(0, bot->getConfigurableIndex()); // done with actors

  // RUN STATE
  bot->cycle(MODE_PRESSED, false, TimingInterruptNone, 0.0f);
  TEST_ASSERT_EQUAL(RunMode, bot->getMode());

  // CONFIG CONFIGURABLES STATE
  bot->cycle(MODE_PRESSED, false, TimingInterruptNone, 0.0f);
  TEST_ASSERT_EQUAL(ConfigConfigurablesMode, bot->getMode());

  // and so on...
}

void test_bot_correctly_switches_infos(void) {
  int nroActors = 1;
  int nroConfigurables = 1;
  TestActor a0("ACTOR0", true);
  Clock clock(nroActors);
  Actor *dumbActors[] = {&a0, 0};           // null terminated
  Configurable *configurables[] = {&a0, 0}; // null terminated
  Bot *bot = new Bot(&clock, dumbActors, configurables);
  bot->setStdoutFunction(displayLcdMockupFunctionString);

  bot->setMode(RunMode);

  TEST_ASSERT_EQUAL(nroActors - 1, bot->getConfigurableIndex()); // dumbActor actor
  TEST_ASSERT_EQUAL(0, bot->getConfigurableStateIndex());        // first dumbActor info state (unique of the dumbActor itself)

  bot->cycle(false, SET_PRESSED, TimingInterruptNone, 0.0f);

  TEST_ASSERT_EQUAL(nroActors - 1, bot->getConfigurableIndex()); // dumbActor actor
  TEST_ASSERT_EQUAL(1, bot->getConfigurableStateIndex());        // second dumbActor info state (last watering time)

  bot->cycle(false, SET_PRESSED, TimingInterruptNone, 0.0f);

  TEST_ASSERT_EQUAL(0, bot->getConfigurableIndex()); // back to the origin
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
