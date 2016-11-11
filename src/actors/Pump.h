#ifndef PUMP_INC
#define PUMP_INC

#include <Log.h>
#include <Misc.h>
#include <actors/Actor.h>

#define MIN_WATER_PUMP_AMOUNT_PER_SHOT 1
#define DEFAULT_WATER_PUMP_AMOUNT_PER_SHOT 3
#define MAX_WATER_PUMP_AMOUNT_PER_SHOT 30
#define INCR_WATER_PUMP_AMOUNT_PER_SHOT 1

enum PumpConfigState {
  PumpConfigStateAmount = 0, // configuration of the amount of water per shot (in seconds of pump on)
  PumpConfigStateDelimiter   // delimiter of the configuration states
};

enum PumpInfoState {
  PumpLastWatered = 0, // info telling how long ago the last watering took place
  PumpInfoDelimiter
};

/**
* This actor aims to activate a pump (for instance for watering purposes).
* Shoots of water happen at a controlled frequency and have a controlled duration.
*/
class Pump : public Actor {

private:
  const char *name;            // name of the current pump
  bool activated;              // flag telling if the pump is activated or not
  int cowPerShot;              // expressed in amount of cycles where the pump is on
  int cowLeft;                 // amount of Cycles Of Watering left where the pump should be on
  long cyclesFromLastWatering; // amount of cycles when last watered

public:
  Pump(const char *name);

  const char *getName();

  void cycle(bool cronMatches);
  int getActuatorValue();

  int getNroConfigs();
  void setConfig(int configIndex, char *retroMsg, bool set);

  void getInfo(int infoIndex, char *retroMsg);
  int getNroInfos();
};

#endif // PUMP_INC
