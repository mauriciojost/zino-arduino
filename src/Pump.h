#ifndef PUMP_INC
#define PUMP_INC

#include <Log.h>
#include <Misc.h>

#define MIN_WATER_PUMP_AMOUNT_PER_SHOT 1
#define DEFAULT_WATER_PUMP_AMOUNT_PER_SHOT 10
#define MAX_WATER_PUMP_AMOUNT_PER_SHOT 60
#define INCR_WATER_PUMP_AMOUNT_PER_SHOT 1

enum PumpState {
  PumpOnState = 0,
  PumpOffState = 1,
  DelimiterAmountOfPumpStates = 2
};

class Pump {

private:
public:
  PumpState state;
  int waterAmountPerShot; // expressed in amount of cycles where the pump is on
  int cyclesOfWateringLeft;

  Pump();
  void cycle(bool mustWaterNow);
  bool isPumpDriven();
  void nextWaterAmountPerShot();
};

#endif // PUMP_INC
