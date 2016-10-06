#ifdef UNIT_TEST
#ifdef MISC_TEST

// Auxiliary libraries
#include <unity.h>
#include <stdio.h>

// Library being tested
#include <Misc.h>

#define ANGLE_FOR_PARKING 0
#define ANGLE_FOR_FRACTION_010 47
#define ANGLE_FOR_FRACTION_005 37


void test_to_hour_minute_seconds_string(void) {
  char buffer[16];

  int second = 1;
  int minute = 60 * second;
  int hour = 60 * minute;

  toHourMinuteSecondsString(hour, buffer);
  TEST_ASSERT_EQUAL_STRING("01:00:00", buffer);

  toHourMinuteSecondsString(minute, buffer);
  TEST_ASSERT_EQUAL_STRING("00:01:00", buffer);

  toHourMinuteSecondsString(second, buffer);
  TEST_ASSERT_EQUAL_STRING("00:00:01", buffer);

  toHourMinuteSecondsString(hour + minute + second, buffer);
  TEST_ASSERT_EQUAL_STRING("01:01:01", buffer);

  toHourMinuteSecondsString(2 * hour + 5 * minute + 10 * second, buffer);
  TEST_ASSERT_EQUAL_STRING("02:05:10", buffer);

}

void test_to_day_hour_minutes_string(void) {
  char buffer[16];

  int second = 1;
  int minute = 60 * second;
  int hour = 60 * minute;
  int day = 24 * hour;

  toDayHourMinutesString(hour, buffer);
  TEST_ASSERT_EQUAL_STRING("0 days 01:00", buffer);

}

void test_water_amounts_mapping(void) {

  TEST_ASSERT_EQUAL(0.0f, fractionPooredWater(0));
  TEST_ASSERT_EQUAL(0.5f, fractionPooredWater(90));
  TEST_ASSERT_EQUAL(1.0f, fractionPooredWater(180));

  TEST_ASSERT_EQUAL(0, angleGivenPooringAmount(0.0f));
  TEST_ASSERT_EQUAL(90, angleGivenPooringAmount(0.5f));
  TEST_ASSERT_EQUAL(180, angleGivenPooringAmount(1.0f));

}

void test_servo_position(void) {
  // trivial cases
  TEST_ASSERT_EQUAL(0, calculateNewServoPosition(0, 0.0f));
  TEST_ASSERT_EQUAL(90, calculateNewServoPosition(0, 0.5f));
  TEST_ASSERT_EQUAL(180, calculateNewServoPosition(0, 1.0f));

  // non trivial cases
  TEST_ASSERT_EQUAL(ANGLE_FOR_FRACTION_005, calculateNewServoPosition(0, 0.05f));
  TEST_ASSERT_EQUAL(ANGLE_FOR_FRACTION_010, calculateNewServoPosition(0, 0.10f));
}

// THIS SHOULD BE PUT SOMEWHERE ELSE


int main() {
  UNITY_BEGIN();
  RUN_TEST(test_to_hour_minute_seconds_string);
  RUN_TEST(test_to_day_hour_minutes_string);
  RUN_TEST(test_water_amounts_mapping);
  RUN_TEST(test_servo_position);
  UNITY_END();
}

#endif
#endif
