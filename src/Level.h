#ifndef LEVEL_INC
#define LEVEL_INC

#include <Log.h>
#include <Misc.h>
#include <Actor.h>

#define INCR_MIN_LEVEL 1
#define MIN_MIN_LEVEL 0
#define MAX_MIN_LEVEL 5

enum LevelConfigState {
  LevelConfigMinimum,       // minimum amount of water in reservoir
  LevelConfigStateDelimiter // delimiter of the configuration states
};

enum LevelInfoState { LevelCurrent = 0, LevelInfoDelimiter };

class Level : public Actor {

private:
  const char *name; // name of the current actor
  int currentLevel;
  bool tooLow;      // true if level of water in reservoir is very low
  int minimumLevel;
  int (*readLevelFunction)();

public:
  Level(const char *name, int (*readLevel)());

  const char *getName();

  void cycle(bool cronMatches);
  int getActuatorValue();

  int getNroConfigs();
  void setConfig(int configIndex, char *retroMsg, bool set);

  void getInfo(int infoIndex, char *retroMsg);
  int getNroInfos();
};

#endif // LEVEL_INC
