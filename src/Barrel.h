#ifndef BARREL_INC
#define BARREL_INC

#include <Log.h>
#include <Misc.h>

#define FULL_FRACTION 1.0f
#define EMPTY_FRACTION 0.0f

#define MIN_WATER_AMOUNT_PER_SHOT 0.05f
#define DEFAULT_WATER_AMOUNT_PER_SHOT 0.10f
#define MAX_WATER_AMOUNT_PER_SHOT 1.00f
#define INCR_WATER_AMOUNT_PER_SHOT 0.05f

enum ServoState {
  ServoReleasedState = 0,
  ServoDrivenState = 1,
  ServoParkingState = 2,
  DelimiterAmountOfServoStates = 3
};

class Barrel {

private:
public:
  ServoState servoState;    // state of the servo
  float waterAmountPerShot; // expressed in fraction of capacity
  float waterCurrentAmount; // expressed in fraction of capacity remaining
  int servoPosition;        // expressed in degrees
  int maxServoPosition;     // expressed in degrees

  Barrel();
  void cycle(bool mustWaterNow);
  bool isServoDriven();
  float remainingWater();
  void nextWaterAmountPerShot();
  void setToFilled();
};

#endif // BARREL_INC
