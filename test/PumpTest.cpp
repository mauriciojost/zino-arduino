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
  TEST_ASSERT_EQUAL(false, p.isDriven());

  p.cycle(NOT_TIME_TO_WATER);

  TEST_ASSERT_EQUAL(false, p.isDriven());

  p.cycle(TIME_TO_WATER);

  for (int t = 0; t < DEFAULT_WATER_PUMP_AMOUNT_PER_SHOT + 1; t++) {
    TEST_ASSERT_EQUAL(true, p.isDriven());
    p.cycle(NOT_TIME_TO_WATER);
  }

  TEST_ASSERT_EQUAL(false, p.isDriven());
}

void test_state_switching(void) {
  bool hasNext = false;
  char buffer[16 + 1];
  Pump p("PUMP");

  hasNext = p.hasNextConfigState(true);
  TEST_ASSERT_EQUAL(true, hasNext);
  TEST_ASSERT_EQUAL(PumpConfigAmountState, p.nextConfigState(buffer));
  TEST_ASSERT_EQUAL(PumpConfigAmountState, p.currentConfigState(buffer));

  hasNext = p.hasNextConfigState(true); // reset again
  TEST_ASSERT_EQUAL(true, hasNext);
  TEST_ASSERT_EQUAL(PumpConfigAmountState, p.nextConfigState(buffer));
  TEST_ASSERT_EQUAL(PumpConfigAmountState, p.currentConfigState(buffer));

  hasNext = p.hasNextConfigState(false); // no reset this time
  TEST_ASSERT_EQUAL(true, hasNext);
  TEST_ASSERT_EQUAL(PumpConfigAmountState2, p.nextConfigState(buffer));
  TEST_ASSERT_EQUAL(PumpConfigAmountState2, p.currentConfigState(buffer));

  hasNext = p.hasNextConfigState(false); // no more config states left
  TEST_ASSERT_EQUAL(false, hasNext);

}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_pump_behaviour);
  RUN_TEST(test_state_switching);
  UNITY_END();
}

#endif // PUMP_TEST
#endif // UNIT_TEST
