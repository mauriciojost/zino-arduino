#ifdef UNIT_TEST
#ifdef CLOCK_TEST

// Auxiliary libraries
#include <unity.h>
#include <stdio.h>

// Library being tested
#include <Clock.h>


void setUp(void) {}

void tearDown(void) {}

void test_clock_correctly_sets_time(void) {
  Clock clock;
  for (int d=0; d<31; d++) {
    for (int h=0; h<24; h++) {
      for (int m=0; m<60; m++) {
        for (int s=0; s<60; s++) {
          clock.set(d, h, m, s);
          TEST_ASSERT_EQUAL(d, clock.getDays());
          TEST_ASSERT_EQUAL(h, clock.getHours());
          TEST_ASSERT_EQUAL(m, clock.getMinutes());
          TEST_ASSERT_EQUAL(s, clock.getSeconds());
        }
      }
    }
  }
}

int count_waterings_in_30days(Frequency f) {
  int count = 0;
  Clock clock;
  clock.setFrequency(f);
  for (int c=0; c<CYCLES_IN_30_DAYS - 1; c++) {
    clock.cycle();
    if (clock.matches()) count++;
  }
  return count;
}

void test_clock_correctly_tells_time_to_water(void) {
  TEST_ASSERT_EQUAL(1, count_waterings_in_30days(OncePerMonth));
  TEST_ASSERT_EQUAL(2, count_waterings_in_30days(TwicePerMonth));
  TEST_ASSERT_EQUAL(5, count_waterings_in_30days(OncePerWeek));
  TEST_ASSERT_EQUAL(10, count_waterings_in_30days(TwicePerWeek));
  TEST_ASSERT_EQUAL(15, count_waterings_in_30days(ThreeTimesPerWeek));
  TEST_ASSERT_EQUAL(30, count_waterings_in_30days(OncePerDay));
  TEST_ASSERT_EQUAL(60, count_waterings_in_30days(TwicePerDay));
}


int main() {
  UNITY_BEGIN();
  RUN_TEST(test_clock_correctly_sets_time);
  RUN_TEST(test_clock_correctly_tells_time_to_water);
  UNITY_END();
}

#endif
#endif
