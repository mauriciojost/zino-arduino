#ifndef BOT_INC
#define BOT_INC

#include <Log.h>
#include <Misc.h>
#include <Clock.h>
#include <actors/Actor.h>
#include <actors/Configurable.h>
#include <ui/Messages.h>

enum BotMode { // this must be aligned with the modesData positions
  RunMode = 0,
  WelcomeMode,
  HelpMode,
  ConfigConfigurablesMode,
  ConfigActorFrequenciesMode,
  DelimiterAmountOfBotModes
};

class Bot;

struct BotModeData {
  const BotMode currentMode;
  void (Bot::*currentModeFunction)(BotModeData *data, bool modePressed, bool setPressed, bool timerInterrupt);
  const char *lcdMessage;
  const BotMode nextMode;
};

/**
* This class represents the controller of the module.
* It controls the configuration cycle, execution and info displays of:
* - the clock (useful for timing purposes)
* - the set of actors (useful for controlling different peripherals like pumps, water level devices, etc.)
* - the LCD display / UI interface
*/
class Bot {

private:
  Clock *clock;        // bot internal clock
  BotMode mode;        // mode of the bot
  Actor **actors;      // actors (pumps, ...)
  int nroActors;       // number of actors
  Configurable **configurables; // configurables (clock, actors, ...)
  int nroConfigurables;       // number of configurables
  bool canChangeMode;  // flag telling if changing the mode is possible
  int configurableIndex;      // index of the current configurable being addressed (for configuration or info display)
  int configurableStateIndex; // index of the current configurable state being addressed (for configuration or info display)
  void (*stdOutWriteStringFunction)(const char *, const char *); // stdout write callback function (two lines, normally thought for a 16x2 LCD)
  void toWelcomeMode(BotModeData *data, bool modePressed, bool setPressed, bool timerInterrupt);
  void toHelpMode(BotModeData *data, bool modePressed, bool setPressed, bool timerInterrupt);
  void toRunMode(BotModeData *data, bool modePressed, bool setPressed, bool timerInterrupt);
  void toConfigConfigurablesMode(BotModeData *data, bool modePressed, bool setPressed, bool timerInterrupt);
  void toConfigActorFrequenciesMode(BotModeData *data, bool modePressed, bool setPressed, bool timerInterrupt);

  void stdOutWriteString(const char *up, const char * down);

  void nextInfoState();
  void nextConfigurableConfigState();
  void nextActor();

  void updateInfo(char *buffer1, char *buffer2);

public:
  // The information about each mode, containing mode index, function, message and next mode
  BotModeData modesData[DelimiterAmountOfBotModes] = { // this must be aligned with the BotMode items
      {RunMode, &Bot::toRunMode, MSG_BOT_STATE_RUN, ConfigConfigurablesMode},
      {WelcomeMode, &Bot::toWelcomeMode, MSG_BOT_STATE_WELCOME, HelpMode},
      {HelpMode, &Bot::toHelpMode, MSG_BOT_STATE_HELP, ConfigConfigurablesMode},
      {ConfigConfigurablesMode, &Bot::toConfigConfigurablesMode, MSG_BOT_STATE_ACTORS, ConfigActorFrequenciesMode},
      {ConfigActorFrequenciesMode, &Bot::toConfigActorFrequenciesMode, MSG_BOT_STATE_FREQUENCIES, RunMode}};

  // Constructor.
  Bot(Clock* clock, Actor **arrayOfActors, int nroActors, Configurable **arrayOfConfigurables, int nroConfigurables);

  void setStdoutFunction(void (*wrSt)(const char *, const char *));

  // Function to execute whenever an even takes place (like a button pressed or a timer interrupt).
  void cycle(bool modeButtonPressed, bool setButtonPressed, bool timerInterrupt);

  void setMode(BotMode newMode);

  int getMode();

  int getConfigurableIndex();

  int getConfigurableStateIndex();

  Clock *getClock();
};

#endif // BOT_INC
