#ifndef LEVEL_INC
#define LEVEL_INC

#include <Log.h>
#include <Misc.h>
#include <actors/Actor.h>

#define INCR_MIN_LEVEL 1
#define MIN_MIN_LEVEL 0
#define MAX_MIN_LEVEL 5
#define DEFAULT_MIN_LEVEL 1

enum LevelConfigState {
  LevelConfigMinimum,       // minimum amount of water in reservoir
  LevelConfigStateDelimiter // delimiter of the configuration states
};

enum LevelInfoState {
  LevelCurrent = 0, // info showing the current level of water in reservoir
  LevelInfoDelimiter
};

/**
* This actor aims to measure the amoint of water in a reservoir and
* notify via the actuator if it is too low.
*/
class Level : public Actor {

private:
  const char *name; // name of the current actor
  int currentLevel; // current water level
  int minimumLevel; // threshold under which the level is considered too low
  bool tooLow;      // true if level of water is too low
  int (*readLevelFunction)();
  Actor *actor; // wrapped actor (if any)

public:
  Level(const char *name, int (*readLevel)());
  Level(const char *name, int (*readLevel)(), Actor *a);

  const char *getName();

  void cycle(bool cronMatches);
  int getActuatorValue();

  int getNroConfigs();
  void setConfig(int configIndex, char *retroMsg, bool set);

  void getInfo(int infoIndex, char *retroMsg);
  int getNroInfos();
};

#endif // LEVEL_INC
