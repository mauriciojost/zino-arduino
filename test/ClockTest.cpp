#ifdef UNIT_TEST
#ifdef CLOCK_TEST

// Auxiliary libraries
#include <unity.h>
#include <stdio.h>

// Library being tested
#include <Clock.h>


void setUp(void) {}

void tearDown(void) {}

void test_clock_correctly_ticks(void) {
  Clock clock;
  for (int h=0; h<24; h++) {
    for (int m=0; m<60; m++) {
      for (int s=0; s<60; s++) {
        clock.set(h, m, s);
        TEST_ASSERT_EQUAL(h, clock.getHours());
        TEST_ASSERT_EQUAL(m, clock.getMinutes());
        TEST_ASSERT_EQUAL(s, clock.getSeconds());
      }
    }
  }

}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_clock_correctly_ticks);
  UNITY_END();
}

#endif
#endif
