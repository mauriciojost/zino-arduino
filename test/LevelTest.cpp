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
#include <actors/TestActor.h>
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
  Level l("LEVEL", getLevel);

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
  Level l("LEVEL", getLevel, &t);

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

  Level l("LEVEL", getLevel, &t);
  TEST_ASSERT_EQUAL(TestActorInfoDelimiter + LevelInfoDelimiter, l.getNroInfos());

  int configIndex = 0;
#ifdef BINARY_LEVEL
#else
  l.setConfig(configIndex++, buffer, false);
  TEST_ASSERT_EQUAL_STRING(MSG_LEVEL_CONFIG_MINIMUM "(0<)1", buffer); // mapping a config on Level
#endif // BINARY_LEVEL

  l.setConfig(configIndex++, buffer, false);
  TEST_ASSERT_EQUAL_STRING("TA_CNF_1", buffer); // mapping a config on TestActor (TA)

  l.setConfig(configIndex++, buffer, false);
  TEST_ASSERT_EQUAL_STRING("TA_CNF_2", buffer); // mapping a config on TestActor (TA)
}

void test_configs_behaviour_with_actor(void) {
  char buffer[LCD_LENGTH + 1];
  TestActor t("TEST");

  Level l("LEVEL", getLevel, &t);
  TEST_ASSERT_EQUAL(TestActorConfigStateDelimiter + LevelConfigStateDelimiter, l.getNroConfigs());

  l.getInfo(0, buffer);
#ifdef BINARY_LEVEL
  TEST_ASSERT_EQUAL_STRING(MSG_LEVEL_INFO_CURRENT_LEVEL "LOW", buffer); // mapping an info on Level
#else
  TEST_ASSERT_EQUAL_STRING(MSG_LEVEL_INFO_CURRENT_LEVEL "0<1?", buffer); // mapping an info on Level
#endif // BINARY_LEVEL

  l.getInfo(1, buffer);
  TEST_ASSERT_EQUAL_STRING("TA_INF_1", buffer); // mapping an info on TestActor (TA)

  l.getInfo(2, buffer);
  TEST_ASSERT_EQUAL_STRING("TA_INF_2", buffer); // mapping an info on TestActor (TA)
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
