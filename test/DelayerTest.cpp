#ifdef UNIT_TEST
#ifdef DELAYER_TEST

#define NOT_TIME_TO_WATER false
#define TIME_TO_WATER true
#define PUMP_OFF false
#define PUMP_ON true

// Auxiliary libraries
#include <unity.h>

// Library being tested
#include <actors/Delayer.h>
#include <actors/TestActor.h>

void setUp(void) {}

void tearDown(void) {}

void test_behaviour_with_offset(void) {

  int offset = 5;
  TestActor ta("PUMP");
  Delayer dta(&ta, offset);

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

#endif // DELAYER_TEST
#endif // UNIT_TEST
