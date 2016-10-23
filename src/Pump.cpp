#include <Pump.h>

Pump::Pump(const char* name) {
  this->waterAmountPerShot = DEFAULT_WATER_PUMP_AMOUNT_PER_SHOT;
  this->on = false;
  this->configState = PumpConfigDelimiter;
  this->cyclesOfWateringLeft = 0;
  this->name = name;
}

const char* Pump::getName() {
  return this->name;
}

void Pump::cycle(bool mustWaterNow) {
  if (mustWaterNow) {
    log(Debug, "  PMP: ON");
    this->on = true;
    this->cyclesOfWateringLeft = this->waterAmountPerShot;
  } else if (this->cyclesOfWateringLeft != 0) {
    log(Debug, "  PMP: ON (STILL)", (int)this->cyclesOfWateringLeft);
    this->on = true;
    this->cyclesOfWateringLeft = constrainValue(
        this->cyclesOfWateringLeft - 1, 0, MAX_WATER_PUMP_AMOUNT_PER_SHOT);
  } else {
    log(Debug, "  PMP: OFF");
    this->on = false;
  }
}

int Pump::isDriven() {
  return this->on;
}

int Pump::currentConfigState(char *retroMsg) {
  switch (this->configState) {
  case (PumpConfigAmountState):
    sprintf(retroMsg, "AMOUNT %s: %d", this->name, this->waterAmountPerShot);
    break;
  case (PumpConfigAmountState2):
    sprintf(retroMsg, "XX %s", this->name);
    break;
  default:
    break;
  }
  return (int)this->configState;
}

bool Pump::hasNextConfigState(bool init) { // even if there is no next config state, you must invoke nextConfigState
  if (init) {
    log(Debug, "  PMP: init config");
    this->configState = PumpConfigDelimiter;
    return true;
  } else {
    return (this->configState + 1) < PumpConfigDelimiter;
  }
}

int Pump::nextConfigState(char *retroMsg) {
  this->configState = (PumpConfigState)rollValue(this->configState + 1, 0, PumpConfigDelimiter);
  return currentConfigState(retroMsg);
}

int Pump::setConfig(char *retroMsg) {
  switch (this->configState) {
  case (PumpConfigAmountState):
    this->waterAmountPerShot = rollValue( this->waterAmountPerShot + INCR_WATER_PUMP_AMOUNT_PER_SHOT,
      MIN_WATER_PUMP_AMOUNT_PER_SHOT, MAX_WATER_PUMP_AMOUNT_PER_SHOT);
    sprintf(retroMsg, "%d", this->waterAmountPerShot);
    return this->waterAmountPerShot;
    break;
  case (PumpConfigAmountState2):
    return 0;
    break;
  default:
    return 0;
    break;
  }
}
