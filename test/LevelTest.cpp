#ifdef UNIT_TEST
#ifdef LEVEL_TEST

// Auxiliary libraries
#include <unity.h>

// Library being tested
#include <Level.h>

#define LEVEL_OK false
#define LEVEL_TOO_LOW true

void setUp(void) {}

void tearDown(void) {}

int currentLevel = 0;
int getLevel() { return currentLevel; }

void test_level_behaviour(void) {
  Level l("LEVEL", getLevel);

  currentLevel = 2;
  l.cycle(true);
  TEST_ASSERT_EQUAL(LEVEL_OK, l.getActuatorValue());

  currentLevel = 1;
  l.cycle(true);
  TEST_ASSERT_EQUAL(LEVEL_TOO_LOW, l.getActuatorValue());

  currentLevel = 0;
  l.cycle(true);
  TEST_ASSERT_EQUAL(LEVEL_TOO_LOW, l.getActuatorValue());
}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_level_behaviour);
  UNITY_END();
}

#endif // LEVEL_TEST
#endif // UNIT_TEST
