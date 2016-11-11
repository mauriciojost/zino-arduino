#include <actors/Level.h>
#include <Messages.h>

Level::Level(const char *n, int (*readLevel)()) {
  name = n;
  tooLow = false;
  minimumLevel = DEFAULT_MIN_LEVEL;
  currentLevel = 0;
  readLevelFunction = readLevel;
}

const char *Level::getName() {
  return name;
}

void Level::cycle(bool cronMatches) {
  if (cronMatches || tooLow) { // if too low, read quickly so that re-filling is ackd immediately
    currentLevel = readLevelFunction();
    tooLow = (currentLevel <= minimumLevel);
  }
  log(Debug, "  MLVL: ", minimumLevel);
  log(Debug, "  LVL: ", currentLevel);
  log(Debug, "  LVLTL: ", tooLow);
}

int Level::getActuatorValue() {
  return tooLow;
}

void Level::setConfig(int configIndex, char *retroMsg, bool set) {
  switch (configIndex) {
    case (LevelConfigMinimum):
      if (set) {
        minimumLevel = rollValue(minimumLevel + INCR_MIN_LEVEL, MIN_MIN_LEVEL, MAX_MIN_LEVEL);
      }
      sprintf(retroMsg, "%s %d", MSG_LEVEL_CONFIG_MINIMUM, minimumLevel);
    default:
      break;
  }
}

int Level::getNroConfigs() {
  return (int)LevelConfigStateDelimiter;
}

void Level::getInfo(int infoIndex, char *retroMsg) {
  switch (infoIndex) {
    case (LevelCurrent):
      sprintf(retroMsg, "%s %02d", MSG_LEVEL_INFO_CURRENT_LEVEL, currentLevel);
      break;
  }
}

int Level::getNroInfos() {
  return (int)LevelInfoDelimiter;
}
