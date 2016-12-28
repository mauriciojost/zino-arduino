/*
 * ZINO: low-budget autonomous plant care system.
 *
 * Copyright 2016 Mauricio Jost
 * Contact: mauriciojostx@gmail.com
 *
 * This file is part of ZINO.
 *
 * ZINO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * ZINO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ZINO. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BOT_INC
#define BOT_INC

#include <Log.h>
#include <Misc.h>
#include <actors/Clock.h>
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
  void (Bot::*currentModeFunction)(BotModeData *data, bool modePressed, bool setPressed, TimingInterrupt timingInterrupt, float subCycle);
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
  Clock *clock;                 // bot internal clock
  BotMode mode;                 // mode of the bot
  Actor **actors;               // null terminated array of actors (pumps, ...)
  int nroActors;                // number of actors
  Configurable **configurables; // null terminated array of configurables (clock, actors, ...)
  int nroConfigurables;         // number of configurables
  bool canChangeMode;           // flag telling if changing the mode is possible
  int configurableIndex;        // index of the current configurable being addressed (for configuration or info display)
  int configurableStateIndex;   // index of the current configurable state being addressed (for configuration or info display)
  void (*stdOutWriteStringFunction)(const char *,
                                    const char *); // stdout write callback function (two lines, normally thought for a 16x2 LCD)
  void toWelcomeMode(BotModeData *data, bool modePressed, bool setPressed, TimingInterrupt timingInterrupt, float subCycle);
  void toHelpMode(BotModeData *data, bool modePressed, bool setPressed, TimingInterrupt timingInterrupt, float subCycle);
  void toRunMode(BotModeData *data, bool modePressed, bool setPressed, TimingInterrupt timingInterrupt, float subCycle);
  void toConfigConfigurablesMode(BotModeData *data, bool modePressed, bool setPressed, TimingInterrupt timingInterrupt, float subCycle);
  void toConfigActorFrequenciesMode(BotModeData *data, bool modePressed, bool setPressed, TimingInterrupt timingInterrupt, float subCycle);

  void nextInfoState();
  void nextConfigurableConfigState();
  void nextActorWithConfigurableFrequency();

  void updateInfo(char *buffer1, char *buffer2);

public:
  // The information about each mode, containing mode index, function, message and next mode
  static BotModeData modesData[]; // this must be aligned with the BotMode items

  // Constructor.
  Bot(Clock *clock, Actor **arrayOfActors, Configurable **arrayOfConfigurables);

  void setStdoutFunction(void (*wrSt)(const char *, const char *));

  // Function to execute whenever an event takes place (like a button pressed or a timer interrupt).
  void cycle(bool modeButtonPressed, bool setButtonPressed, TimingInterrupt timingInterrupt, float subCycle);

  void setMode(BotMode newMode);

  int getMode();

  int getConfigurableIndex();

  int getConfigurableStateIndex();

  Clock *getClock();

  void stdOutWriteString(const char *up, const char *down);

};

#endif // BOT_INC
