#ifndef MISC_INC
#define MISC_INC

#include <math.h>

#define constrainValue(amt, low, high)                                         \
  ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
#define rollValue(amt, low, high)                                              \
  ((amt) < (low) ? (high) : ((amt) > (high) ? (low) : (amt)))

float toRadians(int degrees);
float fractionPooredWater(int maxAngleDegrees);
float fractionRemainingWater(int maxAngleDegrees);
int angleGivenPooringAmount(float targetPooringFraction);
int calculateNewServoPosition(int currentMaxServoPosition,
                              float waterAmountPerShot);

#endif // MISC_INC
