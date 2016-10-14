#include <Misc.h>

float toRadians(int degrees) { return (((float)degrees) / 360) * 2 * M_PI; }

float fractionPooredWater(int maxAngleDegrees) { // when 0 degrees spout is up
                                                 // (so 100%) but when 180
                                                 // degrees spout is down (so
                                                 // 0%)
  float maxAngle = toRadians(maxAngleDegrees);
  float effMaxAngle = maxAngle * 2;
  float segmentArea = (effMaxAngle - sin(effMaxAngle)) / 2;
  float area = M_PI; // area of the circle if R=1
  return segmentArea / area;
}

float fractionRemainingWater(int maxAngleDegrees) {
  return 1.0f - fractionPooredWater(maxAngleDegrees);
}

int angleGivenPooringAmount(float targetPooringFraction) {
  for (int angleInDegrees = 0; angleInDegrees <= 180; angleInDegrees++) {
    if (fractionPooredWater(angleInDegrees) >= targetPooringFraction) {
      return angleInDegrees;
    }
  }
  return 181;
}

int calculateNewServoPosition(int currentMaxServoPosition,
                              float waterAmountPerShot) {
  float alreadyPooredWater = fractionPooredWater(currentMaxServoPosition);
  float targetPooredWater = alreadyPooredWater + waterAmountPerShot;
  return angleGivenPooringAmount(targetPooredWater);
}
