#ifndef BOT_INC
#define BOT_INC

#include <Log.h>
#include <Misc.h>
#include <Clock.h>
#include <Actor.h>
#include <Messages.h>

enum BotState { // this must be aligned with the statesSata positions
  RunState = 0,
  WelcomeState,
  ConfigHourState,
  ConfigMinuteState,
  ConfigActorsState,
  DelimiterAmountOfBotStates
};

enum BotInfo {
  ClockInfo = 0,
  DelimiterBotInfo
};

class Bot;

struct BotStateData {
  const BotState currentState;
  void (Bot::*currentStateFunction)(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt);
  const char *lcdMessage;
  const BotState nextState;
};

class Bot {

private:
  Clock *clock;                                          // bot internal clock
  BotState state;                                        // state of the bot
  Actor **actors;                                        // actors (pumps, ...)
  int nroActors;                                         // number of actors
  bool canChangeMode;                                    // flag telling if changing the mode is possible
  int auxStateIndex;                                        // index of the current actor being configured
  int auxSubstateIndex;                                  // index of the current actor state being configured
  void (*stdOutWriteString)(const char *, const char *); // stdout write callback function (for LCD)

  void toWelcomeState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt);
  void toRunState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt);
  void toConfigActorsState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt);
  void toConfigHourState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt);
  void toConfigMinuteState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt);

  void nextActorConfigState();
  void nextInfoState();
  void updateInfo(char* buffer1, char* buffer2);

public:
  BotStateData statesData[DelimiterAmountOfBotStates] = { // this must be aligned with the BotState items
      {RunState, &Bot::toRunState, MSG_BOT_STATE_RUN, ConfigHourState},
      {WelcomeState, &Bot::toWelcomeState, MSG_BOT_STATE_WELCOME, ConfigHourState},
      {ConfigHourState, &Bot::toConfigHourState, MSG_BOT_STATE_HOUR, ConfigMinuteState},
      {ConfigMinuteState, &Bot::toConfigMinuteState, MSG_BOT_STATE_MINUTE, ConfigActorsState},
      {ConfigActorsState, &Bot::toConfigActorsState, MSG_BOT_STATE_ACTORS, RunState}};

  Bot(void (*wrSt)(const char *, const char *), Actor **actors, int nroActors);
  void cycle(bool modePressed, bool setPressed, bool timerInterrupt);
  void setState(BotState s);
  int getState();
  int getAuxStateIndex();
  int getAuxSubstateIndex();
};

#endif // BOT_INC
