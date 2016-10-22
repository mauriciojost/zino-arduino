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
  TEST_ASSERT_EQUAL(false, p.on);
  TEST_ASSERT_EQUAL(false, p.isDriven());

  p.cycle(NOT_TIME_TO_WATER);

  TEST_ASSERT_EQUAL(false, p.on);
  TEST_ASSERT_EQUAL(false, p.isDriven());

  p.cycle(TIME_TO_WATER);

  for (int t = 0; t < p.waterAmountPerShot + 1; t++) {
    TEST_ASSERT_EQUAL(true, p.on);
    TEST_ASSERT_EQUAL(true, p.isDriven());
    p.cycle(NOT_TIME_TO_WATER);
  }

  TEST_ASSERT_EQUAL(false, p.on);
  TEST_ASSERT_EQUAL(false, p.isDriven());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_pump_behaviour);
  UNITY_END();
}

#endif // PUMP_TEST
#endif // UNIT_TEST
