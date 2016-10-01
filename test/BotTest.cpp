#include <unity.h>
#include <Bot.h>

#define BUTTON0_PRESSED true
#define BUTTON1_PRESSED true
#define WDT_INTERRUPT true

#ifdef UNIT_TEST

void setUp(void) {}

void tearDown(void) {}

void test_bot_correctly_switches_states(void) {
  Bot bot;
  TEST_ASSERT_EQUAL(bot.state, Display);
  bot.run(false, false, false);
  TEST_ASSERT_EQUAL(bot.state, Display);
  bot.run(BUTTON0_PRESSED, false, false);
  TEST_ASSERT_EQUAL(bot.state, Run);
  bot.run(false, BUTTON1_PRESSED, false);
  TEST_ASSERT_EQUAL(bot.state, Display);
}

int main() {
  UNITY_BEGIN(); // IMPORTANT LINE!
  RUN_TEST(test_bot_correctly_switches_states);
  UNITY_END(); // stop unit testing
}

#endif
