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

#define NOT_TIME_TO_WATER false
#define TIME_TO_WATER true
#define PUMP_OFF false
#define PUMP_ON true

// Auxiliary libraries
#include <unity.h>

// Library being tested
#include <actors/Delayer.h>
#include <main4ino/TestActor.h>

void setUp(void) {}

void tearDown(void) {}

void test_behaviour_with_offset(void) {

  int offset = 5;
  TestActor ta("PUMP");
  Delayer dta(offset);
  dta.setActor(&ta);

  TEST_ASSERT_EQUAL(PUMP_OFF, dta.getActuatorValue());
  dta.cycle(NOT_TIME_TO_WATER);

  TEST_ASSERT_EQUAL(PUMP_OFF, dta.getActuatorValue());
  dta.cycle(TIME_TO_WATER);

  for (int t = 0; t < offset - 1; t++) {
    TEST_ASSERT_EQUAL(PUMP_OFF, dta.getActuatorValue());
    dta.cycle(NOT_TIME_TO_WATER);
  }

  TEST_ASSERT_EQUAL(PUMP_ON, dta.getActuatorValue());
  dta.cycle(NOT_TIME_TO_WATER);

  TEST_ASSERT_EQUAL(PUMP_OFF, dta.getActuatorValue());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_behaviour_with_offset);
  UNITY_END();
}

#endif // UNIT_TEST
