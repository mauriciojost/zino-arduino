#include <Level.h>
#include <Messages.h>

Level::Level(const char *n, int (*readLevel)()) {
  name = n;
  tooLow = false;
  minimumLevel = 1;
  readLevelFunction = readLevel;
}

const char *Level::getActorName() { return name; }

void Level::cycle(bool mustActNow) {
  if (mustActNow) {
    int level = readLevelFunction();
    log(Debug, "LEVEL: ", level);
    log(Debug, "MLEVEL: ", minimumLevel);
    tooLow = (level <= minimumLevel);
  }
}

int Level::getActorState() { return tooLow; }

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

int Level::getNroConfigs() { return (int)LevelConfigStateDelimiter; }

void Level::getInfo(int infoIndex, char *retroMsg) {
  switch (infoIndex) {
  case (LevelCurrent):
    sprintf(retroMsg, "%s %02d", MSG_LEVEL_INFO_CURRENT_LEVEL, currentLevel);
    break;
  }
}

int Level::getNroInfos() { return (int)LevelInfoDelimiter; }
