// Auxiliary libraries
#include <unity.h>
#include <iostream>
using namespace std;

// Library being tested
#include <Bot.h>

#define BUTTON0_PRESSED true
#define BUTTON1_PRESSED true
#define WDT_INTERRUPT true

#ifdef UNIT_TEST

void setUp(void) {}

void tearDown(void) {}

const char** lcdContent = NULL;

void displayLcdMockupFunction(const char* str){
  cout << "+----------------+" << endl;
  cout << "|" << str <<    "|" << endl;
  cout << "|" << str + 16 + 1 << "|" << endl;
  cout << "+----------------+" << endl << endl;
  lcdContent = &str;
}

void test_bot_correctly_switches_states(void) {
  Bot bot(displayLcdMockupFunction);

  TEST_ASSERT_EQUAL(bot.state, Welcome); // welcome state
  bot.run(false, false, false);
  TEST_ASSERT_EQUAL(bot.state, Welcome); // keep in the same state
  bot.run(BUTTON0_PRESSED, false, false);
  TEST_ASSERT_EQUAL(bot.state, Run); // to run
  TEST_ASSERT_EQUAL(*lcdContent, lcdMessageRun); // the LCD should always follow
  bot.run(false, BUTTON1_PRESSED, false);
  TEST_ASSERT_EQUAL(bot.state, Welcome); // back to the welcome state
  TEST_ASSERT_EQUAL(*lcdContent, lcdMessageWelcome); // the LCD should always follow
  bot.run(false, BUTTON1_PRESSED, false);
  TEST_ASSERT_EQUAL(bot.state, Config); // to the config state
  TEST_ASSERT_EQUAL(*lcdContent, lcdMessageConfig); // the LCD should always follow

}

int main() {
  UNITY_BEGIN(); // IMPORTANT LINE!
  RUN_TEST(test_bot_correctly_switches_states);
  UNITY_END(); // stop unit testing
}

#endif
