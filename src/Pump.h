#ifndef PUMP_INC
#define PUMP_INC

#include <Log.h>
#include <Misc.h>
#include <Actor.h>

#define MIN_WATER_PUMP_AMOUNT_PER_SHOT 1
#define DEFAULT_WATER_PUMP_AMOUNT_PER_SHOT 3
#define MAX_WATER_PUMP_AMOUNT_PER_SHOT 30
#define INCR_WATER_PUMP_AMOUNT_PER_SHOT 1

enum PumpConfigState {
  PumpConfigStateAmount = 0, // configuration of the amount of water per shot (in seconds of pump on)
  PumpConfigStateDelimiter   // delimiter of the configuration states
};

enum PumpInfoState { PumpLastWatered = 0, PumpInfoDelimiter };

class Pump : public Actor {

private:
  const char *name;         // name of the current pump
  bool on;                  // flag telling if the pump is on or not
  int cowPerShot;// expressed in amount of cycles where the pump is on
  int cowLeft;              // amount of Cycles Of Watering left where the pump should be on
  long cyclesFromLastWatering;  // amount of cycles when last watered

public:
  Pump(const char *name);

  const char *getActorName();

  void cycle(bool mustActNow);
  int getActorState();

  int getNroConfigs();
  void setConfig(int configIndex, char *retroMsg, bool set);

  void getInfo(int infoIndex, char *retroMsg);
  int getNroInfos();
};

#endif // PUMP_INC
