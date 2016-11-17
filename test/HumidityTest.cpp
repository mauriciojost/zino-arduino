#ifdef UNIT_TEST
#ifdef HUMIDITY_TEST

// Auxiliary libraries
#include <unity.h>

// Library being tested
#include <actors/Humidity.h>
#include <actors/TestActor.h>

void setUp(void) {}

void tearDown(void) {}

void test_behaviour(void) {
  TEST_ASSERT_EQUAL(1, 1);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_behaviour);
  UNITY_END();
}

#endif // HUMIDITY_TEST
#endif // UNIT_TEST
