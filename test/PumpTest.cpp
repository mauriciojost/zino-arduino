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

#define NUL -1
#define NOL false

#define NO false
#define YE true
#define SD 1000 // sample duration
#define PUMP_OFF 0
#define DEFAULT_POS 90
#define MAX_SAMPLES 1000

int sampleIndex = 0;
int positions[MAX_SAMPLES];
int durations[MAX_SAMPLES];
bool pumpons[MAX_SAMPLES];

// Auxiliary libraries
#include <unity.h>

// Library being tested
#include <actors/Pump.h>

void controlServoFunction(int pos, int ms, bool pump) {
  printf("POS: %d, MS: %d, PUMP: %d\n", pos, ms, pump);
  positions[sampleIndex] = pos;
  durations[sampleIndex] = ms;
  pumpons[sampleIndex] = pump;
  sampleIndex++;
}

void setUp(void) {
  for (int i = 0; i < MAX_SAMPLES; i++) {
    positions[i] = NUL;
    durations[i] = NUL;
    pumpons[i] = NOL;
  }
}

void tearDown(void) {}

void test_pump_behaviour(void) {
  char buffer[LCD_LENGTH];
  Pump p("PUMP");
  int amount = 10;
  int posVariation = 4;
  p.setServoWriteFunction(controlServoFunction);
  p.setOnValue(DEFAULT_POS);
  p.setConfig(PumpConfigStateAmount, buffer, SetValue, &amount);
  p.setConfig(PumpConfigStateVariationRange, buffer, SetValue, &posVariation);

  p.cycle(true);

  int expectedNroValues = 14;
  //                                           0   1   2   3   4   5   6   7   8   9  10  11  12  13
  int expectedPositions[expectedNroValues] = {90, 90, 92, 94, 92, 90, 88, 86, 88, 90, 92, 94, 94, NUL};
  int expectedDurations[expectedNroValues] = {SD, SD, SD, SD, SD, SD, SD, SD, SD, SD, SD, SD, SD, NUL};
  bool expectedPumpons[expectedNroValues] =  {NO, NO, YE, YE, YE, YE, YE, YE, YE, YE, YE, YE, NO, NOL};

  for (int i = 0; i < expectedNroValues; i++) {
    printf("Cycle %d\n", i);
    TEST_ASSERT_EQUAL(expectedPositions[i], positions[i]);
    TEST_ASSERT_EQUAL(expectedDurations[i], durations[i]);
    TEST_ASSERT_EQUAL(expectedPumpons[i], pumpons[i]);
  }

}

#endif // UNIT_TEST
