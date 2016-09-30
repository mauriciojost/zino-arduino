#include <unity.h>
#include <Bot.h>

#ifdef UNIT_TEST

void setUp(void) {

}

void tearDown(void) {

}

void test_led_builtin_pin_number(void) {
  TEST_ASSERT_EQUAL(13, 13);
}

int main() {
  UNITY_BEGIN(); // IMPORTANT LINE!
  RUN_TEST(test_led_builtin_pin_number);
  UNITY_END(); // stop unit testing
}

#endif
