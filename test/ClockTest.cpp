#ifdef UNIT_TEST
#ifdef CLOCK_TEST

// Auxiliary libraries
#include <unity.h>
#include <stdio.h>

// Library being tested
#include <Clock.h>

void setUp(void) {}

void tearDown(void) {}


bool isFinalCycle(Clock* clock) {
  return
    (clock->getDays() == 29) &&
    (clock->getHours() == 23) &&
    (clock->getMinutes() == 59) &&
    (clock->getSeconds() == 59);
}

void test_clock_advances_time(void) {
  long c = 0;
  int nroActors = 1;
  Clock clock(nroActors);
  while (!isFinalCycle(&clock)) {
    TEST_ASSERT_EQUAL(c, clock.getCyclesFromT0());
    TEST_ASSERT_EQUAL(round(c * secToCyclesFactor), clock.getSecondsFromT0());
    clock.cycle();
    c++;
  }

  int offDay = 0;
  int offHou = 23;
  int offMin = 1;
  int offSec = 2;

  clock.set(offDay, offHou, offMin, offSec);
  long t0 = offDay * 3600 * 24 + offHou * 3600 + offMin * 60 + offSec;

  c = 0;
  while (!isFinalCycle(&clock)) {
    long expectedSecondsFromT0 = round(c * secToCyclesFactor) + t0;
    int expectedDays = expectedSecondsFromT0 / (3600 * 24);
    int expectedHours = (expectedSecondsFromT0 / 3600) % 24;
    int expectedMinutes = (expectedSecondsFromT0 % 3600) / 60;
    int expectedSeconds = (expectedSecondsFromT0 % 60);

    TEST_ASSERT_EQUAL(c, clock.getCyclesFromT0());
    TEST_ASSERT_EQUAL(expectedSecondsFromT0, clock.getSecondsFromT0());
    TEST_ASSERT_EQUAL(expectedDays, clock.getDays());
    TEST_ASSERT_EQUAL(expectedHours, clock.getHours());
    TEST_ASSERT_EQUAL(expectedMinutes, clock.getMinutes());
    TEST_ASSERT_EQUAL(expectedSeconds, clock.getSeconds());

    clock.cycle();
    c++;
  }

  TEST_ASSERT_EQUAL(29, clock.getDays());
  TEST_ASSERT_EQUAL(23, clock.getHours());
  TEST_ASSERT_EQUAL(59, clock.getMinutes());
  TEST_ASSERT_EQUAL(59, clock.getSeconds());

  clock.cycle();

  TEST_ASSERT_EQUAL(0, clock.getDays());
  TEST_ASSERT_EQUAL(0, clock.getHours());
  TEST_ASSERT_EQUAL(0, clock.getMinutes());
  TEST_ASSERT_EQUAL(0, clock.getSeconds());


}

void test_clock_correctly_sets_time(void) {
  int nroActors = 1;
  Clock clock(nroActors);
  for (int d = 0; d < 31; d++) {
    for (int h = 0; h < 24; h++) {
      for (int m = 0; m < 60; m++) {
        for (int s = 0; s < 60; s++) {
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
  int nroActors = 1;
  int configurableIndex = 0;
  Clock clock(nroActors);
  clock.setFrequency(configurableIndex, f);
  while (!isFinalCycle(&clock)) {
    clock.cycle();
    if (clock.matches(configurableIndex)) {
      count++;
    }
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
  RUN_TEST(test_clock_advances_time);
  RUN_TEST(test_clock_correctly_sets_time);
  RUN_TEST(test_clock_correctly_tells_time_to_water);
  UNITY_END();
}

#endif // CLOCK_TEST
#endif // UNIT_TEST
