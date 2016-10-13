#ifndef BOT_INC
#define BOT_INC

#include <Log.h>
#include <Misc.h>
#include <Clock.h>
#include <Barrel.h>
#include <Pump.h>

enum BotState {
  RunState = 0,
  WelcomeState = 1,
  ConfigPeriodState = 2,
  ConfigAmountState = 3,
  ConfigAmountPumpState = 4,
  ConfigHourState = 5,
  ConfigMinuteState = 6,
  ConfigFilledState = 7,
  DelimiterAmountOfBotStates = 8
};

struct BotStateData {
  const BotState currentState;
  const char *lcdMessage;
  const BotState nextState;
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
  void toConfigAmountPumpState(BotStateData data, bool modePressed, bool setPressed,
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

  const BotStateData statesData[DelimiterAmountOfBotStates] = {
    {RunState, "RUNNING...", ConfigPeriodState},
    {WelcomeState, "WELCOME!", ConfigPeriodState},
    {ConfigPeriodState, "FREQUENCY?", ConfigAmountState},
    {ConfigAmountState, "WATER/SHOT?", ConfigAmountPumpState},
    {ConfigAmountPumpState, "WATER/SHOT/PMP?", ConfigHourState},
    {ConfigHourState, "HOUR?", ConfigMinuteState},
    {ConfigMinuteState, "MINUTE?", ConfigFilledState},
    {ConfigFilledState, "JUST FILLED?", RunState}
  };

  Clock clock;              // bot internal clock
  BotState state;           // state of the bot
  Barrel barrel;            // barrel
  Pump pump;                // pump
  void (*stdOutWriteString)(const char *,
                            const char *); // stdout write callback function

  Bot(void (*wrSt)(const char *, const char *));
  void cycle(bool modePressed, bool setPressed, bool timerInterrupt);
  bool isServoDriven();
  bool isPumpDriven();

};

#endif // BOT_INC
