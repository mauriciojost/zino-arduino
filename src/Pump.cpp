#include <Pump.h>

Pump::Pump() {
  this->waterAmountPerShot = DEFAULT_WATER_PUMP_AMOUNT_PER_SHOT;
  this->state = PumpOffState;
  this->cyclesOfWateringLeft = 0;
}

void Pump::cycle(bool mustWaterNow) {
  if (mustWaterNow) {
    log(Debug, "  PMP: ON");
    this->state = PumpOnState;
    this->cyclesOfWateringLeft = this->waterAmountPerShot;
  } else if (this->cyclesOfWateringLeft != 0) {
    log(Debug, "  PMP: ON (STILL)", (int)this->cyclesOfWateringLeft);
    this->state = PumpOnState;
    this->cyclesOfWateringLeft = constrainValue(
        this->cyclesOfWateringLeft - 1, 0, MAX_WATER_PUMP_AMOUNT_PER_SHOT);
  } else {
    log(Debug, "  PMP: OFF");
    this->state = PumpOffState;
  }
}

bool Pump::isPumpDriven() { return (this->state == PumpOnState); }

void Pump::nextWaterAmountPerShot() {
  this->waterAmountPerShot =
      rollValue(this->waterAmountPerShot + INCR_WATER_PUMP_AMOUNT_PER_SHOT,
                MIN_WATER_PUMP_AMOUNT_PER_SHOT, MAX_WATER_PUMP_AMOUNT_PER_SHOT);
}
