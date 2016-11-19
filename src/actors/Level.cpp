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
    currentLevel = readLevelFunction();
    tooLow = (currentLevel <= minimumLevel);
  }

  if (actor != NULL) {
    actor->cycle(tooLow);
  }

  log(CLASS, Debug, "  MLVL: ", minimumLevel);
  log(CLASS, Debug, "  LVL: ", currentLevel);
  log(CLASS, Debug, "  LVLTL: ", tooLow);
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
    case (LevelConfigMinimum):
      if (set) {
        minimumLevel = rollValue(minimumLevel + INCR_MIN_LEVEL, MIN_MIN_LEVEL, MAX_MIN_LEVEL);
      }
      sprintf(retroMsg, "%s%d", MSG_LEVEL_CONFIG_MINIMUM, minimumLevel);
    default:
      if (actor != NULL) {
        actor->setConfig(configIndex - LevelConfigStateDelimiter, retroMsg, set);
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
      sprintf(retroMsg, "%s%02d<=%02d", MSG_LEVEL_INFO_CURRENT_LEVEL, currentLevel, minimumLevel);
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
