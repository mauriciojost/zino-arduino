#include <Pump.h>

Pump::Pump(const char *name) {
  this->waterAmountPerShot = DEFAULT_WATER_PUMP_AMOUNT_PER_SHOT;
  this->on = false;
  this->cyclesOfWateringLeft = 0;
  this->name = name;
}

const char *Pump::getActorName() { return this->name; }

void Pump::cycle(bool mustActNow) {
  if (mustActNow) {
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

int Pump::getActorState() { return this->on; }

void Pump::setConfig(int configIndex, char *retroMsg, bool set) {
  switch (configIndex) {
  case (PumpConfigStateAmount):
    if (set) {
      this->waterAmountPerShot =
          rollValue(this->waterAmountPerShot + INCR_WATER_PUMP_AMOUNT_PER_SHOT, MIN_WATER_PUMP_AMOUNT_PER_SHOT, MAX_WATER_PUMP_AMOUNT_PER_SHOT);
    }
    sprintf(retroMsg, "AMOUNT %s: %d", this->name, this->waterAmountPerShot);
    break;
  case (PumpConfigStateAmount2):
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

int Pump::getNroConfigs() {
  return (int)PumpConfigStateDelimiter;
}
