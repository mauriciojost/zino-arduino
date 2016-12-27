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

Level::Level(const char *n, int (*readLevel)()) {
  name = n;
  tooLow = false;
  minimumLevel = DEFAULT_MIN_LEVEL;
  currentLevel = 0;
  readLevelFunction = readLevel;
  actor = NULL;
}

Level::Level(const char *n) {
  name = n;
  tooLow = false;
  minimumLevel = DEFAULT_MIN_LEVEL;
  currentLevel = 0;
  readLevelFunction = NULL;
  actor = NULL;
}

void Level::setReadLevelFunction(int (*readLevel)()) {
  readLevelFunction = readLevel;
}

Level::Level(const char *n, int (*readLevel)(), Actor *a) {
  name = n;
  tooLow = false;
  minimumLevel = DEFAULT_MIN_LEVEL;
  currentLevel = 0;
  readLevelFunction = readLevel;
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
  }

  if (actor != NULL) {
    actor->cycle(tooLow);
  }

  log(CLASS, Debug, "  MLVL: ", minimumLevel);
  log(CLASS, Debug, "  LVL: ", currentLevel);
  log(CLASS, Debug, "  LVLTL: ", tooLow);
}

void Level::subCycle(float subCycle) {
  if (actor != NULL) {
    actor->subCycle(subCycle);
  }
}

int Level::getActuatorValue() {
  if (actor == NULL) {
    return tooLow;
  } else {
    return actor->getActuatorValue();
  }
}

void Level::setConfig(int configIndex, char *retroMsg, bool set) {
  switch (configIndex) {
#ifdef BINARY_LEVEL
#else
    int level = 0;
    case (LevelConfigMinimum):
      if (set) {
        minimumLevel = rollValue(minimumLevel + INCR_MIN_LEVEL, MIN_MIN_LEVEL, MAX_MIN_LEVEL);
      }
      if (readLevelFunction != NULL) {
        level = readLevelFunction();
      }
      sprintf(retroMsg, "%s(%d<)%d", MSG_LEVEL_CONFIG_MINIMUM, level, minimumLevel);
#endif // BINARY_LEVEL
    default:
      if (actor != NULL) {
        actor->setConfig(configIndex - LevelConfigStateDelimiter, retroMsg, set);
      } else {
        sprintf(retroMsg, "");
      }
      break;
  }
}

int Level::getNroConfigs() {
  if (actor == NULL) {
    return (int)LevelConfigStateDelimiter;
  } else {
    return (int)LevelConfigStateDelimiter + actor->getNroConfigs();
  }
}

void Level::getInfo(int infoIndex, char *retroMsg) {
  switch (infoIndex) {
    case (LevelCurrent):
#ifdef BINARY_LEVEL
      if (currentLevel) {
        sprintf(retroMsg, "%sOK", MSG_LEVEL_INFO_CURRENT_LEVEL);
      } else {
        sprintf(retroMsg, "%sLOW", MSG_LEVEL_INFO_CURRENT_LEVEL);
      }
#else
      sprintf(retroMsg, "%s%d<%d?", MSG_LEVEL_INFO_CURRENT_LEVEL, currentLevel, minimumLevel);
#endif // BINARY_LEVEL
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

bool Level::isFrequencyConfigurable() {
  return false; // use default frequency
}
