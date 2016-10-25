#include <Pump.h>

Pump::Pump(const char *name) {
  this->waterAmountPerShot = DEFAULT_WATER_PUMP_AMOUNT_PER_SHOT;
  this->on = false;
  this->cyclesOfWateringLeft = 0;
  this->name = name;
}

const char *Pump::getName() { return this->name; }

void Pump::cycle(bool mustWaterNow) {
  if (mustWaterNow) {
    log(Debug, "  PMP: ON");
    this->on = true;
    this->cyclesOfWateringLeft = this->waterAmountPerShot;
  } else if (this->cyclesOfWateringLeft != 0) {
    log(Debug, "  PMP: ON (STILL)", (int)this->cyclesOfWateringLeft);
    this->on = true;
    this->cyclesOfWateringLeft = constrainValue(this->cyclesOfWateringLeft - 1, 0, MAX_WATER_PUMP_AMOUNT_PER_SHOT);
  } else {
    log(Debug, "  PMP: OFF");
    this->on = false;
  }
}

int Pump::isDriven() { return this->on; }

void Pump::setConfig(int configState, char *retroMsg, bool set) {
  switch (configState) {
  case (PumpConfigAmountState):
    if (set) {
      this->waterAmountPerShot =
          rollValue(this->waterAmountPerShot + INCR_WATER_PUMP_AMOUNT_PER_SHOT, MIN_WATER_PUMP_AMOUNT_PER_SHOT, MAX_WATER_PUMP_AMOUNT_PER_SHOT);
    }
    sprintf(retroMsg, "AMOUNT %s: %d", this->name, this->waterAmountPerShot);
    break;
  case (PumpConfigAmountState2):
    if (set) {
      this->waterAmountPerShot =
          rollValue(this->waterAmountPerShot + (INCR_WATER_PUMP_AMOUNT_PER_SHOT * 3), MIN_WATER_PUMP_AMOUNT_PER_SHOT, MAX_WATER_PUMP_AMOUNT_PER_SHOT);
    }
    sprintf(retroMsg, "AMOUNT* %s: %d", this->name, this->waterAmountPerShot);
    break;
  default:
    break;
  }
}

int Pump::getNroConfigStates() {
  return (int)PumpConfigDelimiter;
}
