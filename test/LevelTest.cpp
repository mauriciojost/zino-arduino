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
#ifdef LEVEL_TEST

// Auxiliary libraries
#include <unity.h>

// Library being tested
#include <actors/Level.h>
#include <main4ino/TestActor.h>
#include <ui/Messages.h>

#define LEVEL_OK false
#define LEVEL_TOO_LOW true

void setUp(void) {}

void tearDown(void) {}

int currentLevel = 0;
int getLevel() {
  return currentLevel;
}

void test_level_behaviour(void) {
  Level l("LEVEL");
  l.setReadLevelFunction(getLevel);

  currentLevel = 2;
  l.cycle(true);
  TEST_ASSERT_EQUAL(LEVEL_OK, l.getActuatorValue());

  currentLevel = 1;
  l.cycle(true);
  TEST_ASSERT_EQUAL(LEVEL_OK, l.getActuatorValue());

  currentLevel = 0;
  l.cycle(true);
  TEST_ASSERT_EQUAL(LEVEL_TOO_LOW, l.getActuatorValue());
}

void test_level_behaviour_with_actor(void) {
  TestActor t("TEST");
  Level l("LEVEL");
  l.setActor(&t);
  l.setReadLevelFunction(getLevel);

  currentLevel = 2;
  l.cycle(true);
  TEST_ASSERT_EQUAL(LEVEL_OK, l.getActuatorValue());

  currentLevel = 1;
  l.cycle(true);
  TEST_ASSERT_EQUAL(LEVEL_OK, l.getActuatorValue());

  currentLevel = 0;
  l.cycle(true);
  TEST_ASSERT_EQUAL(LEVEL_TOO_LOW, l.getActuatorValue());

  t.clearActuatorValue();                            // changing actuator value of the wrapped actor
  TEST_ASSERT_EQUAL(LEVEL_OK, l.getActuatorValue()); // actuator value actually driven by the wrapped actor
}

void test_infos_behaviour_with_actor(void) {
  char buffer[LCD_LENGTH + 1];
  TestActor t("TEST");

  Level l("LEVEL", true);
  l.setActor(&t);
  l.setReadLevelFunction(getLevel);
  TEST_ASSERT_EQUAL(TestActorInfoDelimiter + LevelInfoDelimiter, l.getNroInfos());

  int configIndex = 0;

  l.setConfig(configIndex++, buffer, DoNotSet); // configuration of the frequency
  l.setConfig(configIndex++, buffer, DoNotSet); // configuration of the advanced mode

  l.setConfig(configIndex++, buffer, DoNotSet);
  TEST_ASSERT_EQUAL_STRING(MSG_LEVEL_CONFIG_MINIMUM "(0<)1", buffer); // mapping a config on Level

  l.setConfig(configIndex++, buffer, DoNotSet);
  TEST_ASSERT_EQUAL_STRING(MSG_LEVEL_CONFIG_MAXIMUM "(0>)3", buffer); // mapping a config on Level

  l.setConfig(configIndex++, buffer, DoNotSet);
  TEST_ASSERT_EQUAL_STRING("TA_CNF_1:0", buffer); // mapping a config on TestActor (TA)

  l.setConfig(configIndex++, buffer, DoNotSet);
  TEST_ASSERT_EQUAL_STRING("TA_CNF_2:0", buffer); // mapping a config on TestActor (TA)
}

void test_configs_behaviour_with_actor(void) {
  char buffer[LCD_LENGTH + 1];
  TestActor t("TEST");

  Level l("LEVEL", true);
  l.setActor(&t);
  l.setReadLevelFunction(getLevel);
  TEST_ASSERT_EQUAL(TestActorConfigStateDelimiter + LevelConfigStateDelimiter, l.getNroConfigs());

  l.getInfo(0, buffer);
  TEST_ASSERT_EQUAL_STRING(MSG_LEVEL_INFO_CURRENT_LEVEL "(1<)0(<3)?", buffer); // mapping an info on Level

  l.getInfo(1, buffer);
  TEST_ASSERT_EQUAL_STRING("TA_INF_1:0", buffer); // mapping an info on TestActor (TA)

  l.getInfo(2, buffer);
  TEST_ASSERT_EQUAL_STRING("TA_INF_2:0", buffer); // mapping an info on TestActor (TA)
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_level_behaviour);
  RUN_TEST(test_level_behaviour_with_actor);
  RUN_TEST(test_infos_behaviour_with_actor);
  RUN_TEST(test_configs_behaviour_with_actor);
  UNITY_END();
}

#endif // LEVEL_TEST
#endif // UNIT_TEST
