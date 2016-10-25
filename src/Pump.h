#ifndef PUMP_INC
#define PUMP_INC

#include <Log.h>
#include <Misc.h>

#define MIN_WATER_PUMP_AMOUNT_PER_SHOT 1
#define DEFAULT_WATER_PUMP_AMOUNT_PER_SHOT 10
#define MAX_WATER_PUMP_AMOUNT_PER_SHOT 60
#define INCR_WATER_PUMP_AMOUNT_PER_SHOT 1

enum PumpConfigState {
  PumpConfigAmountState = 0, // configuration of the amount of water per shot (in seconds of pump on)
  PumpConfigAmountState2,    // unused configuration state
  PumpConfigDelimiter        // delimiter of the configuration states
};

class Pump {

private:
  const char *name;            // name of the current pump
  bool on;                     // flag telling if the pump is on or not
  int waterAmountPerShot;      // expressed in amount of cycles where the pump is on
  int cyclesOfWateringLeft;    // amount of cycles left where the pump should be on

public:
  Pump(const char *name);

  const char *getName();

  void cycle(bool mustWaterNow);
  int isDriven();

  int getNroConfigStates();
  void setConfig(int configState, char *retroMsg, bool set);

};

#endif // PUMP_INC
