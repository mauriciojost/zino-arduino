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

#ifndef LEVEL_INC
#define LEVEL_INC

#include <log4ino/Log.h>
#include <main4ino/Actor.h>
#include <main4ino/Misc.h>

#define INCR_MIN_LEVEL 1
#define MIN_MIN_LEVEL 0
#define MAX_MIN_LEVEL 3
#define DEFAULT_MIN_LEVEL 1

#define INCR_MAX_LEVEL 1
#define MIN_MAX_LEVEL 0
#define MAX_MAX_LEVEL 3
#define DEFAULT_MAX_LEVEL 3

enum LevelConfigState {
  LevelConfigFrequency = 0, // frequency at which checks will occur
  LevelAdvancedConfig, // advanced config state
  LevelConfigMinimum, // minimum amount of water in reservoir
  LevelConfigMaximum, // maximum amount of water in reservoir
  LevelConfigStateDelimiter // delimiter of the configuration states
};

enum LevelInfoState {
  LevelCurrent = 0, // info showing the current level of water in reservoir
  LevelInfoDelimiter
};

/**
* This actor aims to measure the amoint of water in a reservoir and
* notify via the actuator if it is too low.
* Note: will not be serializable if contains a sub-actor.
*/
class Level : public Actor {

private:
  char name[NAME_LEN + 1];        // name of the current actor
  int currentLevel; // current water level
  int minimumLevel; // threshold under which the level is considered too low
  int maximumLevel; // threshold over which the level is considered too high
  bool tooLow;      // true if level of water is too low
  bool tooHigh;      // true if level of water is too high
  int (*readLevelFunction)();
  Actor *actor;      // wrapped actor (if any)
  FreqConf freqConf; // configuration of the frequency at which this actor will get triggered
  bool advancedConfig; // advanced configuration flag

public:
  Level(const char *name, int (*readLevel)() = NULL, Actor *a = NULL, bool advConfig = false);

  void setReadLevelFunction(int (*readLevel)());

  const char *getName();

  void cycle(bool cronMatches);
  void subCycle(float subCycle);
  int getActuatorValue();

  int getNroConfigs();
  void setConfig(int configIndex, char *retroMsg, bool set);

  void getInfo(int infoIndex, char *retroMsg);
  int getNroInfos();

  FreqConf *getFrequencyConfiguration();
};

#endif // LEVEL_INC
