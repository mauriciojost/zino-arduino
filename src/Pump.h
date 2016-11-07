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
  const char *name; // name of the current pump
  bool on;          // flag telling if the pump is on or not
  int cowPerShot;   // expressed in amount of cycles where the pump is on
  int cowLeft;      // amount of Cycles Of Watering left where the pump should be on

  int cowOffset; // offset used to start watering some cycles after the match (useful to avoid having electrical load peaks when activating the pumps)

  bool shouldHaveWatered;      // tells if the watering should have been triggered (but was not due to the offset imposed)
  int shouldHaveWateredAgo;    // tells how long ago (in cycles) the watering should have been triggered if offset was zero
  long cyclesFromLastWatering; // amount of cycles when last watered

public:
  Pump(const char *name, int cowOffs);

  const char *getName();

  void cycle(bool cronMatches);
  int getActuatorValue();

  int getNroConfigs();
  void setConfig(int configIndex, char *retroMsg, bool set);

  void getInfo(int infoIndex, char *retroMsg);
  int getNroInfos();
};

#endif // PUMP_INC
