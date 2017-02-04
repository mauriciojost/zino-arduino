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
#ifdef PUMP_TEST

#define NOT_TIME_TO_WATER false
#define TIME_TO_WATER true
#define PUMP_OFF 0
#define PUMP_ON 1

// Auxiliary libraries
#include <unity.h>

// Library being tested
#include <actors/Pump.h>

void setUp(void) {}

void tearDown(void) {}

void test_pump_behaviour(void) {
  char buffer[LCD_LENGTH];
  Pump p("PUMP");
  p.setOnValue(PUMP_ON);

  p.setConfig(PumpConfigStateAmount, buffer, true); // DEFAULT_WATER_PUMP_AMOUNT_PER_SHOT + 1
  p.setConfig(PumpConfigStateAmount, buffer, true); // DEFAULT_WATER_PUMP_AMOUNT_PER_SHOT + 2
  p.setConfig(PumpConfigStateAmount, buffer, true); // DEFAULT_WATER_PUMP_AMOUNT_PER_SHOT + 3

  TEST_ASSERT_EQUAL(PUMP_OFF, p.getActuatorValue());

  p.cycle(NOT_TIME_TO_WATER);

  TEST_ASSERT_EQUAL(PUMP_OFF, p.getActuatorValue());

  p.cycle(TIME_TO_WATER);

  TEST_ASSERT_EQUAL(-PUMP_ON, p.getActuatorValue()); // pump off (negative values are used to position the servo)
  p.cycle(NOT_TIME_TO_WATER);
  for (int t = 0; t < DEFAULT_WATER_PUMP_AMOUNT_PER_SHOT + 3; t++) {
    TEST_ASSERT_EQUAL(PUMP_ON, p.getActuatorValue());
    p.cycle(NOT_TIME_TO_WATER);
  }

  TEST_ASSERT_EQUAL(PUMP_OFF, p.getActuatorValue());
}

void test_pump_behaviour_with_disperser(void) {
  char buffer[LCD_LENGTH];
  int onValue = 10;
  Pump p("PUMP");
  p.setOnValue(onValue);

  p.setConfig(PumpConfigStateAmount, buffer, true); // DEFAULT_WATER_PUMP_AMOUNT_PER_SHOT + 1
  p.setConfig(PumpConfigStateAmount, buffer, true); // DEFAULT_WATER_PUMP_AMOUNT_PER_SHOT + 2
  p.setConfig(PumpConfigStateAmount, buffer, true); // DEFAULT_WATER_PUMP_AMOUNT_PER_SHOT + 3

  TEST_ASSERT_EQUAL(PUMP_OFF, p.getActuatorValue());

  p.cycle(NOT_TIME_TO_WATER);

  TEST_ASSERT_EQUAL(PUMP_OFF, p.getActuatorValue());

  p.cycle(TIME_TO_WATER);

  TEST_ASSERT_EQUAL(-(onValue + 0), p.getActuatorValue()); // pump still off during this cycle
  p.subCycle(0.00f);
  TEST_ASSERT_EQUAL(-(onValue + ON_VALUE_DISPERSER_INC), p.getActuatorValue());
  p.subCycle(0.25f);
  TEST_ASSERT_EQUAL(-(onValue + 0), p.getActuatorValue());
  p.subCycle(0.50f);
  TEST_ASSERT_EQUAL(-(onValue - ON_VALUE_DISPERSER_INC), p.getActuatorValue());
  p.subCycle(0.75f);
  p.cycle(NOT_TIME_TO_WATER);
  for (int t = 0; t < DEFAULT_WATER_PUMP_AMOUNT_PER_SHOT + 3; t++) { // pump on during these cycles
    TEST_ASSERT_EQUAL(onValue + 0, p.getActuatorValue());
    p.subCycle(0.00f);
    TEST_ASSERT_EQUAL(onValue + ON_VALUE_DISPERSER_INC, p.getActuatorValue());
    p.subCycle(0.25f);
    TEST_ASSERT_EQUAL(onValue + 0, p.getActuatorValue());
    p.subCycle(0.50f);
    TEST_ASSERT_EQUAL(onValue - ON_VALUE_DISPERSER_INC, p.getActuatorValue());
    p.subCycle(0.75f);
    p.cycle(NOT_TIME_TO_WATER);
  }

  TEST_ASSERT_EQUAL(PUMP_OFF, p.getActuatorValue());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_pump_behaviour);
  RUN_TEST(test_pump_behaviour_with_disperser);
  UNITY_END();
}

#endif // PUMP_TEST
#endif // UNIT_TEST
