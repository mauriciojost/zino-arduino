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

#include <actors/Level.h>
#include <ui/Messages.h>

#define CLASS "Level"

Level::Level(const char *n, bool advConfig): freqConf(OncePerDay, OncePerHour) {
  strncpy(name, n, NAME_LEN);
  readLevelFunction = NULL;
  actor = NULL;
  minimumLevel = DEFAULT_MIN_LEVEL;
  maximumLevel = DEFAULT_MAX_LEVEL;
  tooLow = false;
  tooHigh = false;
  currentLevel = 0;
  advancedConfig = advConfig;
}

void Level::setReadLevelFunction(int (*readLevel)()) {
  readLevelFunction = readLevel;
}

void Level::setActor(Actor* a) {
  actor = a;
}

const char *Level::getName() {
  if (actor == NULL) {
    return name;
  } else {
    return actor->getName();
  }
}

void Level::cycle(bool cronMatches) {
  if (cronMatches || tooLow) { // if too low, read quickly so that changes are ackd immediately
    if (readLevelFunction != NULL) {
      currentLevel = readLevelFunction();
    }
    tooLow = (currentLevel < minimumLevel);
    tooHigh = (currentLevel > maximumLevel);
  }

  if (actor != NULL) {
    actor->cycle(tooLow || tooHigh);
  }

  log(CLASS, Debug, "  MINLVL: ", minimumLevel);
  log(CLASS, Debug, "  MAXLVL: ", maximumLevel);
  log(CLASS, Debug, "  LVL: ", currentLevel);
  log(CLASS, Debug, "  LOWLVL: ", tooLow);
  log(CLASS, Debug, "  HIGLVL: ", tooHigh);
}

int Level::getActuatorValue() {
  if (actor == NULL) {
    return tooLow;
  } else {
    return actor->getActuatorValue();
  }
}

void Level::setConfig(int configIndex, char *retroMsg, SetMode set, int* value) {
  int level = 0;
  switch (configIndex) {
    case (LevelConfigFrequency):
      if (set == SetNext) {
        freqConf.setNextFrequency();
      }
      sprintf(retroMsg, "%s%s", MSG_FREQ, freqConf.getFrequencyDescription());
      break;
    case (LevelAdvancedConfig):
      if (set == SetNext) {
        advancedConfig = !advancedConfig;
      }
      sprintf(retroMsg, "%s%s", MSG_LEVEL_CONFIG_ADVANCED, (advancedConfig ? MSG_YES : MSG_NO));
      break;
    case (LevelConfigMinimum):
      if (set == SetNext) {
        minimumLevel = rollValue(minimumLevel + INCR_MIN_LEVEL, MIN_MIN_LEVEL, MAX_MIN_LEVEL);
      }
      if (readLevelFunction != NULL) {
        level = readLevelFunction();
      }
      sprintf(retroMsg, "%s(%d<)%d", MSG_LEVEL_CONFIG_MINIMUM, level, minimumLevel);
      break;
    case (LevelConfigMaximum):
      if (set == SetNext) {
        maximumLevel = rollValue(maximumLevel + INCR_MAX_LEVEL, MIN_MAX_LEVEL, MAX_MAX_LEVEL);
      }
      if (readLevelFunction != NULL) {
        level = readLevelFunction();
      }
      sprintf(retroMsg, "%s(%d>)%d", MSG_LEVEL_CONFIG_MAXIMUM, level, maximumLevel);
      break;
    default:
      if (actor != NULL) {
        actor->setConfig(configIndex - LevelConfigStateDelimiter, retroMsg, set, value);
      } else {
        retroMsg[0] = 0;
      }
      break;
  }
}

int Level::getNroConfigs() {
  if (advancedConfig) {
    if (actor == NULL) {
      return (int)LevelConfigStateDelimiter;
    } else {
      return (int)LevelConfigStateDelimiter + actor->getNroConfigs();
    }
  } else {
    return LevelAdvancedConfig + 1;
  }
}

void Level::getInfo(int infoIndex, char *retroMsg) {
  switch (infoIndex) {
    case (LevelCurrent):
      sprintf(retroMsg, "%s(%d<)%d(<%d)?", MSG_LEVEL_INFO_CURRENT_LEVEL, minimumLevel, currentLevel, maximumLevel);
      break;
    default:
      if (actor != NULL) {
        actor->getInfo(infoIndex - LevelInfoDelimiter, retroMsg);
      }
      break;
  }
}

int Level::getNroInfos() {
  if (actor == NULL) {
    return (int)LevelInfoDelimiter;
  } else {
    return (int)LevelInfoDelimiter + actor->getNroInfos();
  }
}

FreqConf *Level::getFrequencyConfiguration() {
  if (actor == NULL) {
    return &freqConf;
  } else {
    return actor->getFrequencyConfiguration();
  }
}
