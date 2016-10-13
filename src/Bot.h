#ifndef BOT_INC
#define BOT_INC

#include <Log.h>
#include <Misc.h>
#include <Clock.h>
#include <Barrel.h>

#define INTERNAL_PERIOD_TO_SECONDS_FACTOR 8.0f

enum BotState {
  RunState = 0,
  WelcomeState = 1,
  ConfigPeriodState = 2,
  ConfigAmountState = 3,
  ConfigHourState = 4,
  ConfigMinuteState = 5,
  ConfigFilledState = 6,
  DelimiterAmountOfBotStates = 7
};

struct BotStateData {
  BotState currentState;
  const char *lcdMessage;
  BotState nextState;
};

class Bot {

private:
  void doTransition(BotState toState, bool modePressed, bool setPressed,
                    bool timerInterrupt);

  void toWelcomeState(BotStateData data, bool modePressed, bool setPressed,
                      bool timerInterrupt);
  void toRunState(BotStateData data, bool modePressed, bool setPressed,
                  bool timerInterrupt);
  void toConfigPeriodState(BotStateData data, bool modePressed, bool setPressed,
                           bool timerInterrupt);
  void toConfigAmountState(BotStateData data, bool modePressed, bool setPressed,
                           bool timerInterrupt);
  void toConfigHourState(BotStateData data, bool modePressed, bool setPressed,
                         bool timerInterrupt);
  void toConfigMinuteState(BotStateData data, bool modePressed, bool setPressed,
                           bool timerInterrupt);
  void toConfigFilledState(BotStateData data, bool modePressed, bool setPressed,
                           bool timerInterrupt);
  void toServoTestState(BotStateData data, bool modePressed, bool setPressed,
                           bool timerInterrupt);

public:
  Clock clock;              // bot internal clock
  BotState state;           // state of the bot
  Barrel barrel;           // barrel
  void (*stdOutWriteString)(const char *,
                            const char *); // stdout write callback function

  Bot(void (*wrSt)(const char *, const char *));
  void cycle(bool modePressed, bool setPressed, bool timerInterrupt);
  bool isServoDriven();

};

#endif // BOT_INC
