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
  TEST_ASSERT_EQUAL_STRING(statesData[WelcomeState].lcdMessage, *lcdContentUp);

  bot.run(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigPeriodState, bot.state);
  TEST_ASSERT_EQUAL_STRING(statesData[ConfigPeriodState].lcdMessage, *lcdContentUp);

  bot.run(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(ConfigAmountState, bot.state);
  TEST_ASSERT_EQUAL_STRING(statesData[ConfigAmountState].lcdMessage, *lcdContentUp);

  bot.run(MODE_PRESSED, false, false);
  TEST_ASSERT_EQUAL(RunState, bot.state);
  TEST_ASSERT_EQUAL_STRING(statesData[RunState].lcdMessage, *lcdContentUp);

}


void test_bot_correctly_initializes_servo(void) {

  Bot bot(displayLcdMockupFunctionString);

  bot.state = RunState;

  TEST_ASSERT_EQUAL(false, bot.isServoDriven); // not driven

  for (int i=0; i<EXTRA_WATERING_CYCLES; i++) {
    bot.run(BUTTON_NOT_PRESSED, BUTTON_NOT_PRESSED, TIME_GOES_ON);
    TEST_ASSERT_EQUAL(true, bot.isServoDriven); // driven (driving servo for some cycles)
    TEST_ASSERT_EQUAL(0, bot.servoPosition);
  }

  for (int i=0; i<EXTRA_PARKING_CYCLES; i++) {
    bot.run(BUTTON_NOT_PRESSED, BUTTON_NOT_PRESSED, TIME_GOES_ON);
    TEST_ASSERT_EQUAL(true, bot.isServoDriven); // driven (parking servo for some cycles)
    TEST_ASSERT_EQUAL(ANGLE_FOR_PARKING, bot.servoPosition);
  }

  bot.run(BUTTON_NOT_PRESSED, BUTTON_NOT_PRESSED, TIME_GOES_ON);
  TEST_ASSERT_EQUAL(false, bot.isServoDriven); // not driven (servo parked)
  TEST_ASSERT_EQUAL(ANGLE_FOR_PARKING, bot.servoPosition);

}

void test_bot_correctly_waters(void) {

  Bot bot(displayLcdMockupFunctionString);

  bot.state = RunState;

  for (int i=0; i<=5; i++) {
    bot.run(BUTTON_NOT_PRESSED, BUTTON_NOT_PRESSED, TIME_GOES_ON);
  }

  TEST_ASSERT_EQUAL(false, bot.isServoDriven); // now servo should be already parked

  bot.waterTimerCounter = 10000; // programatically trick to force the watering

  for (int i=0; i<EXTRA_WATERING_CYCLES + 1; i++) {
    bot.run(BUTTON_NOT_PRESSED, BUTTON_NOT_PRESSED, TIME_GOES_ON);
    TEST_ASSERT_EQUAL(true, bot.isServoDriven); // driving servo for watering for some cycles
    TEST_ASSERT_EQUAL(ANGLE_FOR_FRACTION_010, bot.servoPosition);
    TEST_ASSERT_EQUAL(i, bot.waterTimerCounter); // was reset already
  }


  for (int i=0; i<EXTRA_PARKING_CYCLES; i++) {
    bot.run(BUTTON_NOT_PRESSED, BUTTON_NOT_PRESSED, TIME_GOES_ON);
    TEST_ASSERT_EQUAL(true, bot.isServoDriven); // parking servo for some cycles
    TEST_ASSERT_EQUAL(ANGLE_FOR_PARKING, bot.servoPosition);
  }

  bot.run(BUTTON_NOT_PRESSED, BUTTON_NOT_PRESSED, TIME_GOES_ON);
  TEST_ASSERT_EQUAL(false, bot.isServoDriven); // not driven (servo parked)
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
