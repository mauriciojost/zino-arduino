#ifndef BOT_INC
#define BOT_INC

#include <Log.h>
#include <Misc.h>
#include <Clock.h>
#include <Pump.h>

enum BotState {
  RunState = 0,
  WelcomeState,
  ConfigPeriodState,
  ConfigActorsState,
  ConfigHourState,
  ConfigMinuteState,
  DelimiterAmountOfBotStates
};

class Bot;

struct BotStateData {
  const BotState currentState;
  void (Bot::*currentStateFunction)(BotStateData data, bool modePressed,
                                    bool setPressed, bool timerInterrupt);
  const char *lcdMessage;
  const BotState nextState;
};

class Bot {

private:
  void toWelcomeState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt);
  void toRunState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt);
  void toConfigPeriodState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt);
  void toConfigActorsState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt);
  void toConfigHourState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt);
  void toConfigMinuteState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt);

public:
  BotStateData statesData[DelimiterAmountOfBotStates] = {
      {RunState, &Bot::toRunState, "RUNNING...", ConfigPeriodState},
      {WelcomeState, &Bot::toWelcomeState, "WELCOME!", ConfigPeriodState},
      {ConfigPeriodState, &Bot::toConfigPeriodState, "FREQUENCY?", ConfigActorsState},
      {ConfigActorsState, &Bot::toConfigActorsState, "ACTORS", ConfigHourState},
      {ConfigHourState, &Bot::toConfigHourState, "HOUR?", ConfigMinuteState},
      {ConfigMinuteState, &Bot::toConfigMinuteState, "MINUTE?", RunState}};

  Clock clock;    // bot internal clock
  BotState state; // state of the bot
  Pump** actors;      // actors (pumps, ...)
  int nroActors;      // number of actors
  bool changeModeEnabled;
  bool changeActorEnabled;
  int actorIndex;
  void (*stdOutWriteString)(const char *, const char *); // stdout write callback function

  Bot(void (*wrSt)(const char *, const char *), Pump** actors, int nroActors);
  void cycle(bool modePressed, bool setPressed, bool timerInterrupt);

};

#endif // BOT_INC
