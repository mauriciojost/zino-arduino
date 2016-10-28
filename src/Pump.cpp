#include <Pump.h>

Pump::Pump(const char *n) {
  name = n;
  on = false;
  cyclesOfWateringLeft = 0;
  waterAmountPerShot = DEFAULT_WATER_PUMP_AMOUNT_PER_SHOT;
}

const char *Pump::getActorName() { return name; }

void Pump::cycle(bool mustActNow) {
  if (mustActNow) {
    log(Debug, "  PMP: ON");
    on = true;
    cyclesOfWateringLeft = waterAmountPerShot;
  } else if (cyclesOfWateringLeft != 0) {
    log(Debug, "  PMP: ON (STILL)", (int)cyclesOfWateringLeft);
    on = true;
    cyclesOfWateringLeft = constrainValue(cyclesOfWateringLeft - 1, 0, MAX_WATER_PUMP_AMOUNT_PER_SHOT);
  } else {
    log(Debug, "  PMP: OFF");
    on = false;
  }
}

int Pump::getActorState() { return on; }

void Pump::setConfig(int configIndex, char *retroMsg, bool set) {
  switch (configIndex) {
  case (PumpConfigStateAmount):
    if (set) {
      waterAmountPerShot =
          rollValue(waterAmountPerShot + INCR_WATER_PUMP_AMOUNT_PER_SHOT, MIN_WATER_PUMP_AMOUNT_PER_SHOT, MAX_WATER_PUMP_AMOUNT_PER_SHOT);
    }
    sprintf(retroMsg, "AMOUNT %s: %d", name, waterAmountPerShot);
    break;
  case (PumpConfigStateAmount2):
    if (set) {
      waterAmountPerShot =
          rollValue(waterAmountPerShot + (INCR_WATER_PUMP_AMOUNT_PER_SHOT * 3), MIN_WATER_PUMP_AMOUNT_PER_SHOT, MAX_WATER_PUMP_AMOUNT_PER_SHOT);
    }
    sprintf(retroMsg, "AMOUNT* %s: %d", name, waterAmountPerShot);
    break;
  default:
    break;
  }
}

int Pump::getNroConfigs() { return (int)PumpConfigStateDelimiter; }
