// Auxiliary libraries
#include <unity.h>
#include <stdio.h>

// Library being tested
#include <Bot.h>

#define MODE_PRESSED true
#define SET_PRESSED true
#define WDT_INTERRUPT true
#define TIME_GOES_ON true

#ifdef UNIT_TEST

const char* EMPTY_MSG = "";

const char **lcdContentUp = &EMPTY_MSG;
const char **lcdContentDown = &EMPTY_MSG;

void setUp(void) {
  lcdContentUp = &EMPTY_MSG;
  lcdContentDown = &EMPTY_MSG;
}

void tearDown(void) {}

void displayLcdMockupFunctionString(const char *str1, const char *str2) {

  lcdContentUp = &str1;
  lcdContentDown = &str2;

  printf("----------------\n");
  printf("%s\n", *lcdContentUp);
  printf("%s\n", *lcdContentDown);
  printf("----------------\n");
}

void test_bot_correctly_switches_states(void) {

  Bot bot(displayLcdMockupFunctionString);

  TEST_ASSERT_EQUAL(bot.state, WelcomeState);

  bot.run(false, false, false);
  TEST_ASSERT_EQUAL(bot.state, WelcomeState);
  TEST_ASSERT_EQUAL_STRING(*lcdContentUp, "WELCOME");

  bot.run(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(bot.state, ConfigPeriodState);
  TEST_ASSERT_EQUAL_STRING(*lcdContentUp, "WATER EVERY...?");

  bot.run(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(bot.state, ConfigAmountState);
  TEST_ASSERT_EQUAL_STRING(*lcdContentUp, "HOW MUCH WATER?");

}


void test_bot_correctly_initializes_servo(void) {

  Bot bot(displayLcdMockupFunctionString);

  bot.run(MODE_PRESSED, false, false); // config period state
  bot.run(MODE_PRESSED, false, false); // config amount state
  bot.run(MODE_PRESSED, false, false); // run state

  TEST_ASSERT_EQUAL(bot.state, RunState);

  TEST_ASSERT_EQUAL(bot.isServoDriven, false); // not driven at t=0

  for (int i=1; i<=2; i++) {
    bot.run(false, false, TIME_GOES_ON);
    TEST_ASSERT_EQUAL(bot.isServoDriven, true); // driven at t=1,2 (driving servo for 2 cycles)
    TEST_ASSERT_EQUAL(bot.servoPosition, 0);
  }

  for (int i=3; i<=4; i++) {
    bot.run(false, false, TIME_GOES_ON);
    TEST_ASSERT_EQUAL(bot.isServoDriven, true); // driven at t=3,4 (parking servo for 2 cycles)
    TEST_ASSERT_EQUAL(bot.servoPosition, 0);
  }

  bot.run(false, false, TIME_GOES_ON);
  TEST_ASSERT_EQUAL(bot.isServoDriven, false); // not driven at t=5 (servo parked)
  TEST_ASSERT_EQUAL(bot.servoPosition, 0);

}

void test_bot_correctly_waters(void) {

  Bot bot(displayLcdMockupFunctionString);

  bot.run(MODE_PRESSED, false, false); // config period state
  bot.run(MODE_PRESSED, false, false); // config amount state
  bot.run(MODE_PRESSED, false, false); // run state

  TEST_ASSERT_EQUAL(bot.state, RunState);

  for (int i=1; i<=100; i++) {
    bot.run(false, false, TIME_GOES_ON);
  }

  TEST_ASSERT_EQUAL(bot.waterTimerCounter, 100); // here is where the bot is in time
  bot.waterTimerCounter = 10000; // programatically trick to force the watering

  for (int i=101; i<=103; i++) {
    bot.run(false, false, TIME_GOES_ON);
    TEST_ASSERT_EQUAL(bot.isServoDriven, true); // driven at t=101,103 (driving servo for 3 cycles)
    TEST_ASSERT_EQUAL(bot.servoPosition, 10);
  }

  TEST_ASSERT_EQUAL(bot.waterTimerCounter, 2); // was reset already

  for (int i=104; i<=105; i++) {
    bot.run(false, false, TIME_GOES_ON);
    TEST_ASSERT_EQUAL(bot.isServoDriven, true); // driven at t=104,105 (parking servo for 2 cycles)
    TEST_ASSERT_EQUAL(bot.servoPosition, 0);
  }

  bot.run(false, false, TIME_GOES_ON);
  TEST_ASSERT_EQUAL(bot.isServoDriven, false); // not driven (servo parked)
  TEST_ASSERT_EQUAL(bot.servoPosition, 0);


}

// THIS SHOULD BE PUT SOMEWHERE ELSE
#include <Misc.h>


void test_to_hour_minute_seconds_string(void) {
  char buffer[16];

  int second = 1;
  int minute = 60 * second;
  int hour = 60 * minute;

  toHourMinuteSecondsString(hour, buffer);
  TEST_ASSERT_EQUAL_STRING(buffer, "01:00:00");

  toHourMinuteSecondsString(minute, buffer);
  TEST_ASSERT_EQUAL_STRING(buffer, "00:01:00");

  toHourMinuteSecondsString(second, buffer);
  TEST_ASSERT_EQUAL_STRING(buffer, "00:00:01");

  toHourMinuteSecondsString(hour + minute + second, buffer);
  TEST_ASSERT_EQUAL_STRING(buffer, "01:01:01");

  toHourMinuteSecondsString(2 * hour + 5 * minute + 10 * second, buffer);
  TEST_ASSERT_EQUAL_STRING(buffer, "02:05:10");

}

void test_to_day_hour_minutes_string(void) {
  char buffer[16];

  int second = 1;
  int minute = 60 * second;
  int hour = 60 * minute;
  int day = 24 * hour;

  toDayHourMinutesString(hour, buffer);
  TEST_ASSERT_EQUAL_STRING(buffer, "0 days 01:00");

}
// THIS SHOULD BE PUT SOMEWHERE ELSE


int main() {
  UNITY_BEGIN(); // IMPORTANT LINE!
  RUN_TEST(test_bot_correctly_switches_states);
  RUN_TEST(test_to_hour_minute_seconds_string);
  RUN_TEST(test_to_day_hour_minutes_string);
  RUN_TEST(test_bot_correctly_waters);
  RUN_TEST(test_bot_correctly_initializes_servo);
  UNITY_END(); // stop unit testing
}

#endif
