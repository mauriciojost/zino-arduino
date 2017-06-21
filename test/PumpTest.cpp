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

#define OF false
#define ON true
#define S1 500 // sample duration
#define S2 (S1 * 2)
#define S4 (S2 * 2)
#define PUMP_OFF 0
#define MAX_SAMPLES 1000

int sampleIndex = 0;
int positions[MAX_SAMPLES];
int durations[MAX_SAMPLES];
bool pumpons[MAX_SAMPLES];
bool smooths[MAX_SAMPLES];

// Auxiliary libraries
#include <unity.h>

// Library being tested
#include <actors/Pump.h>

void controlServoFunction(int pos, int ms, bool pump, bool smooth) {
  printf("POS: %d, MS: %d, PUMP: %d, SMOOTH: %d \n", pos, ms, pump, smooth);
  positions[sampleIndex] = pos;
  durations[sampleIndex] = ms;
  pumpons[sampleIndex] = pump;
  smooths[sampleIndex] = smooth;
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
  int amount = 5;
  int posVariation = 2;
  int defaultPos = 90;
  p.setServoWriteFunction(controlServoFunction);
  p.setOnValue(defaultPos);
  p.setConfig(PumpConfigStateAmount, buffer, SetValue, &amount);
  p.setConfig(PumpConfigStateVariationRange, buffer, SetValue, &posVariation);

  p.cycle(true);

  int expectedNroValues = 13;
  //             active                            x   x   x   x   x   x   x   x   x   x
  //             cycle number                  0   1   2   3   4   5   6   7   8   9  10  11  12
  int expectedPositions[expectedNroValues] = {90, 91, 92, 91, 90, 89, 88, 89, 90, 91, 92, 90, NUL};
  int expectedDurations[expectedNroValues] = {S4, S1, S1, S1, S1, S1, S1, S1, S1, S1, S1, S2, NUL};
  bool expectedPumpons[expectedNroValues] =  {OF, ON, ON, ON, ON, ON, ON, ON, ON, ON, ON, OF, NOL};
  bool expectedSmooths[expectedNroValues] =  {ON, OF, OF, OF, OF, OF, OF, OF, OF, OF, OF, ON, NOL};

  for (int i = 0; i < expectedNroValues; i++) {
    printf("Cycle %d\n", i);
    TEST_ASSERT_EQUAL(expectedPositions[i], positions[i]);
    TEST_ASSERT_EQUAL(expectedDurations[i], durations[i]);
    TEST_ASSERT_EQUAL(expectedPumpons[i], pumpons[i]);
    TEST_ASSERT_EQUAL(expectedSmooths[i], smooths[i]);
  }

}

void test_pump_serialization(void) {
  char buffer[LCD_LENGTH];
  Pump p("PUMP");
  p.setOnValue(0x77665544);
  p.setConfig(PumpConfigStateFrequency, buffer, SetNext, NULL);
  unsigned char ser[p.size()];
  unsigned char serExpected[p.size()] =
  //   0123 4 5 6 7 8 9   name
      "PUMP\0\0\0\0\0\0"
  //   10  11  12  13     onValue
      "\x44\x55\x66\x77"
  //   14  15  16  17     cowPerShot
      "\x01\x00\x00\x00"
  //   18  19  20  21     onValueDisperserRange
      "\x04\x00\x00\x00"
  //   22  23  24  25     freqConf.freq
      "\x01\x00\x00\x00"
  //   26  27  28  29     freqConf.maxFreq
      "\x08\x00\x00\x00"
  //   30  31  32  33     freqConf.matchInvalidateCounter
      "\x00\x00\x00\x00"
      ;
  p.serialize(ser);
  TEST_ASSERT_EQUAL_MEMORY(&serExpected, &ser, p.size());

  Pump q("pump");
  q.deserialize(ser);
  TEST_ASSERT_EQUAL_STRING(p.getName(), q.getName());
  TEST_ASSERT_EQUAL(p.getOnValue(), q.getOnValue());
  TEST_ASSERT_EQUAL(p.getFrequencyConfiguration()->getFrequency(), q.getFrequencyConfiguration()->getFrequency());

}

#endif // UNIT_TEST
