#include <unity.h>

#ifdef UNIT_TEST
#define LED_BUILTIN 13

// void setUp(void) {
// // set stuff up here
// }

// void tearDown(void) {
// // clean stuff up here
// }

void test_led_builtin_pin_number(void) { TEST_ASSERT_EQUAL(LED_BUILTIN, 13); }

int main(char **args) {
  UNITY_BEGIN(); // IMPORTANT LINE!
  RUN_TEST(test_led_builtin_pin_number);
  UNITY_END(); // stop unit testing
}

#endif
