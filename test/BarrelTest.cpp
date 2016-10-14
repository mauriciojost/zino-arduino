#ifdef UNIT_TEST
#ifdef BARREL_TEST

#define TIME_GOES_ON false
#define HAS_TO_WATER true
#define ANGLE_FOR_PARKING 0
#define ANGLE_FOR_FRACTION_010 47
#define ANGLE_FOR_FRACTION_005 37

// Auxiliary libraries
#include <unity.h>

// Library being tested
#include <Barrel.h>

void setUp(void) {}

void tearDown(void) {}

void test_1(void) {
  Barrel b;

  TEST_ASSERT_EQUAL(ServoDrivenState,
                    b.servoState); // servo initializes being driven for a cycle
  TEST_ASSERT_EQUAL(ANGLE_FOR_PARKING, b.servoPosition);

  b.cycle(TIME_GOES_ON);

  TEST_ASSERT_EQUAL(ServoParkingState,
                    b.servoState); // driving servo for parking for a cycle
  TEST_ASSERT_EQUAL(ANGLE_FOR_PARKING, b.servoPosition);

  b.cycle(TIME_GOES_ON);

  TEST_ASSERT_EQUAL(ServoReleasedState, b.servoState); // released now
  TEST_ASSERT_EQUAL(ANGLE_FOR_PARKING, b.servoPosition);

  b.cycle(HAS_TO_WATER);

  TEST_ASSERT_EQUAL(ServoDrivenState,
                    b.servoState); // driving servo for watering for some cycles
  TEST_ASSERT_EQUAL(ANGLE_FOR_FRACTION_010, b.servoPosition);

  b.cycle(TIME_GOES_ON);

  TEST_ASSERT_EQUAL(ServoParkingState,
                    b.servoState); // parking servo for some cycles
  TEST_ASSERT_EQUAL(ANGLE_FOR_PARKING, b.servoPosition);

  b.cycle(TIME_GOES_ON);

  TEST_ASSERT_EQUAL(ServoReleasedState,
                    b.servoState); // not driven (servo parked)
  TEST_ASSERT_EQUAL(ANGLE_FOR_PARKING, b.servoPosition);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_1);
  UNITY_END();
}

#endif // BARREL_TEST
#endif // UNIT_TEST
