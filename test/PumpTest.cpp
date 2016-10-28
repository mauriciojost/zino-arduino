#ifdef UNIT_TEST
#ifdef PUMP_TEST

#define NOT_TIME_TO_WATER false
#define TIME_TO_WATER true

// Auxiliary libraries
#include <unity.h>

// Library being tested
#include <Pump.h>

void setUp(void) {}

void tearDown(void) {}

void test_pump_behaviour(void) {
  Pump p("PUMP");
  TEST_ASSERT_EQUAL(false, p.getActorState());

  p.cycle(NOT_TIME_TO_WATER);

  TEST_ASSERT_EQUAL(false, p.getActorState());

  p.cycle(TIME_TO_WATER);

  for (int t = 0; t < DEFAULT_WATER_PUMP_AMOUNT_PER_SHOT; t++) {
    TEST_ASSERT_EQUAL(true, p.getActorState());
    p.cycle(NOT_TIME_TO_WATER);
  }

  TEST_ASSERT_EQUAL(false, p.getActorState());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_pump_behaviour);
  UNITY_END();
}

#endif // PUMP_TEST
#endif // UNIT_TEST
