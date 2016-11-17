#ifdef UNIT_TEST
#ifdef LEVEL_TEST

// Auxiliary libraries
#include <unity.h>

// Library being tested
#include <actors/Level.h>
#include <actors/TestActor.h>
#include <ui/Messages.h>

#define LEVEL_OK false
#define LEVEL_TOO_LOW true

void setUp(void) {}

void tearDown(void) {}

int currentLevel = 0;
int getLevel() {
  return currentLevel;
}

void test_level_behaviour(void) {
  Level l("LEVEL", getLevel);

  currentLevel = 2;
  l.cycle(true);
  TEST_ASSERT_EQUAL(LEVEL_OK, l.getActuatorValue());

  currentLevel = 1;
  l.cycle(true);
  TEST_ASSERT_EQUAL(LEVEL_OK, l.getActuatorValue());

  currentLevel = 0;
  l.cycle(true);
  TEST_ASSERT_EQUAL(LEVEL_TOO_LOW, l.getActuatorValue());

}

void test_level_behaviour_with_actor(void) {
  TestActor t("TEST");
  Level l("LEVEL", getLevel, &t);

  currentLevel = 2;
  l.cycle(true);
  TEST_ASSERT_EQUAL(LEVEL_OK, l.getActuatorValue());

  currentLevel = 1;
  l.cycle(true);
  TEST_ASSERT_EQUAL(LEVEL_OK, l.getActuatorValue());

  currentLevel = 0;
  l.cycle(true);
  TEST_ASSERT_EQUAL(LEVEL_TOO_LOW, l.getActuatorValue());

  t.clearActuatorValue(); // changing actuator value of the wrapped actor
  TEST_ASSERT_EQUAL(LEVEL_OK, l.getActuatorValue()); // actuator value actually driven by the wrapped actor
}

void test_infos_behaviour_with_actor(void) {
  char buffer[LCD_LENGTH + 1];
  TestActor t("TEST");

  Level l("LEVEL", getLevel, &t);
  TEST_ASSERT_EQUAL(TestActorInfoDelimiter + LevelInfoDelimiter, l.getNroInfos());

  l.setConfig(0, buffer, false);
  TEST_ASSERT_EQUAL_STRING(MSG_LEVEL_CONFIG_MINIMUM " 0", buffer); // mapping a config on Level

  l.setConfig(1, buffer, false);
  TEST_ASSERT_EQUAL_STRING("TA_CNF_1", buffer); // mapping a config on TestActor (TA)

  l.setConfig(2, buffer, false);
  TEST_ASSERT_EQUAL_STRING("TA_CNF_2", buffer); // mapping a config on TestActor (TA)

}

void test_configs_behaviour_with_actor(void) {
  char buffer[LCD_LENGTH + 1];
  TestActor t("TEST");

  Level l("LEVEL", getLevel, &t);
  TEST_ASSERT_EQUAL(TestActorConfigStateDelimiter + LevelConfigStateDelimiter, l.getNroConfigs());

  l.getInfo(0, buffer);
  TEST_ASSERT_EQUAL_STRING(MSG_LEVEL_INFO_CURRENT_LEVEL " 00<=00", buffer); // mapping an info on Level

  l.getInfo(1, buffer);
  TEST_ASSERT_EQUAL_STRING("TA_INF_1", buffer); // mapping an info on TestActor (TA)

  l.getInfo(2, buffer);
  TEST_ASSERT_EQUAL_STRING("TA_INF_2", buffer); // mapping an info on TestActor (TA)

}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_level_behaviour);
  RUN_TEST(test_level_behaviour_with_actor);
  RUN_TEST(test_infos_behaviour_with_actor);
  RUN_TEST(test_configs_behaviour_with_actor);
  UNITY_END();
}

#endif // LEVEL_TEST
#endif // UNIT_TEST
