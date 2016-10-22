#ifndef PUMP_INC
#define PUMP_INC

#include <Log.h>
#include <Misc.h>

#define MIN_WATER_PUMP_AMOUNT_PER_SHOT 1
#define DEFAULT_WATER_PUMP_AMOUNT_PER_SHOT 10
#define MAX_WATER_PUMP_AMOUNT_PER_SHOT 60
#define INCR_WATER_PUMP_AMOUNT_PER_SHOT 1

enum PumpConfigState {
  PumpConfigAmountState = 0,
  PumpConfigAmountState2,
  PumpConfigDelimiter
};

class Pump {

private:

public:

  bool on;
  PumpConfigState configState;
  const char* name;
  int waterAmountPerShot; // expressed in amount of cycles where the pump is on
  int cyclesOfWateringLeft;

  Pump(const char* name);

  const char* getName();

  void cycle(bool mustWaterNow);
  int isDriven();

  bool hasNextConfigState(bool init);
  void nextConfigState(char *retroMsg);
  void setConfig(char* retroMsg);

};

#endif // PUMP_INC
