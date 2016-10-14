#ifdef UNIT_TEST
#ifdef BARREL_TEST

// Auxiliary libraries
#include <unity.h>

// Library being tested
#include <Barrel.h>

void setUp(void) {}

void tearDown(void) {}

void test_1(void) {
  TEST_ASSERT_EQUAL(1, 0 + 1);
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_1);
  UNITY_END();
}

#endif // BARREL_TEST
#endif // UNIT_TEST
