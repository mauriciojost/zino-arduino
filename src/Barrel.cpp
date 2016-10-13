#include <Barrel.h>

Barrel::Barrel(){
  this->waterAmountPerShot = DEFAULT_WATER_AMOUNT_PER_SHOT;
  this->waterCurrentAmount = FULL_FRACTION;
  this->servoState = ServoDrivenState;
  this->servoPosition = 0;
  this->maxServoPosition = 0;
}

float Barrel::remainingWater() {
  return fractionRemainingWater(this->maxServoPosition);
}

void Barrel::nextWaterAmountPerShot() {
  this->waterAmountPerShot = rollValue(this->waterAmountPerShot + INCR_WATER_AMOUNT_PER_SHOT,
            MIN_WATER_AMOUNT_PER_SHOT, MAX_WATER_AMOUNT_PER_SHOT);
}

void Barrel::setToFilled() {
  this->maxServoPosition = 0;
}

void Barrel::cycle(bool mustWaterNow) {
  if (this->servoState == ServoDrivenState) {
    log(Debug, "  SVO: ->PKG");
    this->servoPosition = 0;
    this->servoState = ServoParkingState;
  } else if (this->servoState == ServoParkingState) {
    log(Debug, "  SVO: ->RLS");
    this->servoPosition = 0;
    this->servoState = ServoReleasedState;
  } else if (mustWaterNow) {
    log(Debug, "  SVO: ->WAT");
    this->maxServoPosition = calculateNewServoPosition(
        this->maxServoPosition, this->waterAmountPerShot);
    this->servoPosition = this->maxServoPosition;
    this->servoState = ServoDrivenState;
  } else {
    log(Debug, "  SVO: zzz");
    this->servoState = ServoReleasedState;
  }
  log(Debug, "  SVO: state ", this->servoState);
  log(Debug, "  SVO: position ", this->servoPosition);
}
