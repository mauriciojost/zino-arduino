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
#define PUMP_OFF false
#define PUMP_ON true

// Auxiliary libraries
#include <unity.h>

// Library being tested
#include <actors/Pump.h>

void setUp(void) {}

void tearDown(void) {}

void test_pump_behaviour(void) {
  char buffer[LCD_LENGTH];
  Pump p("PUMP");

  p.setConfig(PumpConfigStateAmount, buffer, true); // DEFAULT_WATER_PUMP_AMOUNT_PER_SHOT + 1
  p.setConfig(PumpConfigStateAmount, buffer, true); // DEFAULT_WATER_PUMP_AMOUNT_PER_SHOT + 2
  p.setConfig(PumpConfigStateAmount, buffer, true); // DEFAULT_WATER_PUMP_AMOUNT_PER_SHOT + 3

  TEST_ASSERT_EQUAL(PUMP_OFF, p.getActuatorValue());

  p.cycle(NOT_TIME_TO_WATER);

  TEST_ASSERT_EQUAL(PUMP_OFF, p.getActuatorValue());

  p.cycle(TIME_TO_WATER);

  for (int t = 0; t < DEFAULT_WATER_PUMP_AMOUNT_PER_SHOT + 3; t++) {
    TEST_ASSERT_EQUAL(PUMP_ON, p.getActuatorValue());
    p.cycle(NOT_TIME_TO_WATER);
  }

  TEST_ASSERT_EQUAL(PUMP_OFF, p.getActuatorValue());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_pump_behaviour);
  UNITY_END();
}

#endif // PUMP_TEST
#endif // UNIT_TEST
