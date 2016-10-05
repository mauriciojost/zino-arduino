// Auxiliary libraries
#include <unity.h>
#include <stdio.h>

// Library being tested
#include <Bot.h>

#define MODE_PRESSED true
#define SET_PRESSED true
#define WDT_INTERRUPT true
#define TIME_GOES_ON true
#define ANGLE_FOR_FRACTION_10 47

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

  TEST_ASSERT_EQUAL(WelcomeState, bot.state);

  bot.run(false, false, false);
  TEST_ASSERT_EQUAL(WelcomeState, bot.state);
  TEST_ASSERT_EQUAL_STRING("WELCOME", *lcdContentUp);

  bot.run(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigPeriodState, bot.state);
  TEST_ASSERT_EQUAL_STRING("WATER EVERY...?", *lcdContentUp);

  bot.run(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigAmountState, bot.state);
  TEST_ASSERT_EQUAL_STRING("WATER/SHOT?", *lcdContentUp);

}


void test_bot_correctly_initializes_servo(void) {

  Bot bot(displayLcdMockupFunctionString);

  bot.run(MODE_PRESSED, false, false); // config period state
  bot.run(MODE_PRESSED, false, false); // config amount state
  bot.run(MODE_PRESSED, false, false); // run state

  TEST_ASSERT_EQUAL(RunState, bot.state);

  TEST_ASSERT_EQUAL(false, bot.isServoDriven); // not driven at t=0

  for (int i=1; i<=2; i++) {
    bot.run(false, false, TIME_GOES_ON);
    TEST_ASSERT_EQUAL(true, bot.isServoDriven); // driven at t=1,2 (driving servo for 2 cycles)
    TEST_ASSERT_EQUAL(0, bot.servoPosition);
  }

  for (int i=3; i<=4; i++) {
    bot.run(false, false, TIME_GOES_ON);
    TEST_ASSERT_EQUAL(true, bot.isServoDriven); // driven at t=3,4 (parking servo for 2 cycles)
    TEST_ASSERT_EQUAL(0, bot.servoPosition);
  }

  bot.run(false, false, TIME_GOES_ON);
  TEST_ASSERT_EQUAL(false, bot.isServoDriven); // not driven at t=5 (servo parked)
  TEST_ASSERT_EQUAL(0, bot.servoPosition);

}

void test_bot_correctly_waters(void) {

  Bot bot(displayLcdMockupFunctionString);

  bot.state = RunState;

  for (int i=0; i<=5; i++) {
    bot.run(false, false, TIME_GOES_ON);
  }

  TEST_ASSERT_EQUAL(false, bot.isServoDriven); // now servo should be already parked

  bot.waterTimerCounter = 10000; // programatically trick to force the watering

  for (int i=0; i<3; i++) {
    bot.run(false, false, TIME_GOES_ON);
    TEST_ASSERT_EQUAL(true, bot.isServoDriven); // driving servo for 3 cycles
    TEST_ASSERT_EQUAL(ANGLE_FOR_FRACTION_10, bot.servoPosition);
  }

  TEST_ASSERT_EQUAL(2, bot.waterTimerCounter); // was reset already

  for (int i=0; i<2; i++) {
    bot.run(false, false, TIME_GOES_ON);
    TEST_ASSERT_EQUAL(true, bot.isServoDriven); // parking servo for 2 cycles
    TEST_ASSERT_EQUAL(0, bot.servoPosition);
  }

  bot.run(false, false, TIME_GOES_ON);
  TEST_ASSERT_EQUAL(false, bot.isServoDriven); // not driven (servo parked)
  TEST_ASSERT_EQUAL(0, bot.servoPosition);


}

// THIS SHOULD BE PUT SOMEWHERE ELSE
#include <Misc.h>


void test_to_hour_minute_seconds_string(void) {
  char buffer[16];

  int second = 1;
  int minute = 60 * second;
  int hour = 60 * minute;

  toHourMinuteSecondsString(hour, buffer);
  TEST_ASSERT_EQUAL_STRING("01:00:00", buffer);

  toHourMinuteSecondsString(minute, buffer);
  TEST_ASSERT_EQUAL_STRING("00:01:00", buffer);

  toHourMinuteSecondsString(second, buffer);
  TEST_ASSERT_EQUAL_STRING("00:00:01", buffer);

  toHourMinuteSecondsString(hour + minute + second, buffer);
  TEST_ASSERT_EQUAL_STRING("01:01:01", buffer);

  toHourMinuteSecondsString(2 * hour + 5 * minute + 10 * second, buffer);
  TEST_ASSERT_EQUAL_STRING("02:05:10", buffer);

}

void test_to_day_hour_minutes_string(void) {
  char buffer[16];

  int second = 1;
  int minute = 60 * second;
  int hour = 60 * minute;
  int day = 24 * hour;

  toDayHourMinutesString(hour, buffer);
  TEST_ASSERT_EQUAL_STRING("0 days 01:00", buffer);

}

void test_water_amounts_mapping(void) {

  TEST_ASSERT_EQUAL(0.0f, fractionPooredWater(0));
  TEST_ASSERT_EQUAL(0.5f, fractionPooredWater(90));
  TEST_ASSERT_EQUAL(1.0f, fractionPooredWater(180));

  TEST_ASSERT_EQUAL(0, angleGivenPooringAmount(0.0f));
  TEST_ASSERT_EQUAL(90, angleGivenPooringAmount(0.5f));
  TEST_ASSERT_EQUAL(180, angleGivenPooringAmount(1.0f));

}

void test_servo_position(void) {
  // trivial cases
  TEST_ASSERT_EQUAL(0, calculateNewServoPosition(0, 0.0f));
  TEST_ASSERT_EQUAL(90, calculateNewServoPosition(0, 0.5f));
  TEST_ASSERT_EQUAL(180, calculateNewServoPosition(0, 1.0f));

  // non trivial cases
  TEST_ASSERT_EQUAL(37, calculateNewServoPosition(0, 0.05f));
  TEST_ASSERT_EQUAL(47, calculateNewServoPosition(0, 0.10f));
}

// THIS SHOULD BE PUT SOMEWHERE ELSE


int main() {
  UNITY_BEGIN(); // IMPORTANT LINE!
  RUN_TEST(test_bot_correctly_switches_states);
  RUN_TEST(test_to_hour_minute_seconds_string);
  RUN_TEST(test_to_day_hour_minutes_string);
  RUN_TEST(test_bot_correctly_waters);
  RUN_TEST(test_bot_correctly_initializes_servo);
  RUN_TEST(test_water_amounts_mapping);
  RUN_TEST(test_servo_position);
  UNITY_END(); // stop unit testing
}

#endif
