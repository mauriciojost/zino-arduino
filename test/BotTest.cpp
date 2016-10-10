#ifdef UNIT_TEST
#ifdef BOT_TEST

// Auxiliary libraries
#include <unity.h>
#include <stdio.h>

// Library being tested
#include <Bot.h>


#define MODE_PRESSED true
#define SET_PRESSED true
#define BUTTON_NOT_PRESSED false
#define WDT_INTERRUPT true
#define TIME_GOES_ON true

#define ANGLE_FOR_PARKING 0
#define ANGLE_FOR_FRACTION_010 47
#define ANGLE_FOR_FRACTION_005 37

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

  printf("\nLCD:\n----------------\n");
  printf("%s\n", *lcdContentUp);
  printf("%s\n", *lcdContentDown);
  printf("----------------\n\n\n");
}

void test_bot_correctly_switches_states(void) {

  Bot bot(displayLcdMockupFunctionString);

  TEST_ASSERT_EQUAL(WelcomeState, bot.state);

  bot.run(BUTTON_NOT_PRESSED, BUTTON_NOT_PRESSED, false);
  TEST_ASSERT_EQUAL(WelcomeState, bot.state);
  TEST_ASSERT_EQUAL_STRING("WELCOME!", *lcdContentUp);

  bot.run(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigPeriodState, bot.state);

  bot.run(false, false, false); // nothing pressed
  TEST_ASSERT_EQUAL(ConfigPeriodState, bot.state);

  bot.run(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigAmountState, bot.state);

}


void test_bot_correctly_initializes_servo(void) {

  Bot bot(displayLcdMockupFunctionString);
  bot.state = RunState;

  for (int i=0; i<1; i++) {
    bot.run(BUTTON_NOT_PRESSED, BUTTON_NOT_PRESSED, TIME_GOES_ON);
    TEST_ASSERT_EQUAL(ServoParkingState, bot.servoState); // driven (driving servo for some cycles)
    TEST_ASSERT_EQUAL(0, bot.servoPosition);
  }

  bot.run(BUTTON_NOT_PRESSED, BUTTON_NOT_PRESSED, TIME_GOES_ON);
  TEST_ASSERT_EQUAL(ServoReleasedState, bot.servoState); // not driven (servo parked)
  TEST_ASSERT_EQUAL(ANGLE_FOR_PARKING, bot.servoPosition);

}

void test_bot_correctly_waters(void) {

  Bot bot(displayLcdMockupFunctionString);
  bot.state = RunState;
  bot.clock.set(0, 12, 0, 0); // so that no watering is triggered
  bot.clock.setFrequency(OncePerDay);

  for (int i=0; i<5; i++) {
    bot.run(BUTTON_NOT_PRESSED, BUTTON_NOT_PRESSED, TIME_GOES_ON); // make time pass
  }

  bot.clock.disableAntiBouncing();
  bot.clock.set(0, 23, 59, 60 + 10); // programatic trick to force the watering

  for (int i=0; i<1; i++) {
    bot.run(BUTTON_NOT_PRESSED, BUTTON_NOT_PRESSED, TIME_GOES_ON);
    TEST_ASSERT_EQUAL(ServoDrivenState, bot.servoState); // driving servo for watering for some cycles
    TEST_ASSERT_EQUAL(ANGLE_FOR_FRACTION_010, bot.servoPosition);
  }


  for (int i=0; i<1; i++) {
    bot.run(BUTTON_NOT_PRESSED, BUTTON_NOT_PRESSED, TIME_GOES_ON);
    TEST_ASSERT_EQUAL(ServoParkingState, bot.servoState); // parking servo for some cycles
    TEST_ASSERT_EQUAL(ANGLE_FOR_PARKING, bot.servoPosition);
  }

  bot.run(BUTTON_NOT_PRESSED, BUTTON_NOT_PRESSED, TIME_GOES_ON);
  TEST_ASSERT_EQUAL(ServoReleasedState, bot.servoState); // not driven (servo parked)
  TEST_ASSERT_EQUAL(ANGLE_FOR_PARKING, bot.servoPosition);


}

int main() {
  UNITY_BEGIN();
  RUN_TEST(test_bot_correctly_switches_states);
  RUN_TEST(test_bot_correctly_waters);
  RUN_TEST(test_bot_correctly_initializes_servo);
  UNITY_END();
}

#endif
#endif
