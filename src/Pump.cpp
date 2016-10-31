#include <Pump.h>
#include <Messages.h>

Pump::Pump(const char *n) {
  name = n;
  on = false;
  cyclesOfWateringLeft = 0;
  waterAmountPerShot = DEFAULT_WATER_PUMP_AMOUNT_PER_SHOT;
  cyclesFromWatering = 0;
}

const char *Pump::getActorName() { return name; }

void Pump::cycle(bool mustActNow) {
  cyclesFromWatering++;
  if (mustActNow) {
    log(Debug, "  PMP: ON");
    on = true;
    cyclesOfWateringLeft = waterAmountPerShot - 1;
    cyclesFromWatering = 0;
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
      waterAmountPerShot = rollValue(waterAmountPerShot + INCR_WATER_PUMP_AMOUNT_PER_SHOT, MIN_WATER_PUMP_AMOUNT_PER_SHOT, MAX_WATER_PUMP_AMOUNT_PER_SHOT);
    }
    sprintf(retroMsg, "%s %ds", MSG_PUMP_CONFIG_AMOUNT, waterAmountPerShot);
    break;
  default:
    break;
  }
}

int Pump::getNroConfigs() { return (int)PumpConfigStateDelimiter; }

void Pump::getInfo(int infoIndex, char *retroMsg) {
  switch (infoIndex) {
  case (PumpLastWatered):
    sprintf(retroMsg, "%s %02dh(cyc)", MSG_PUMP_INFO_LAST_WATERING, cyclesFromWatering / 3600);
    break;
  }
}

int Pump::getNroInfos() { return (int)PumpInfoDelimiter; }
