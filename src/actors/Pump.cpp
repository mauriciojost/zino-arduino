#include <actors/Pump.h>
#include <ui/Messages.h>

Pump::Pump(const char *n) {
  name = n;
  on = false;
  cowLeft = 0;
  cowPerShot = DEFAULT_WATER_PUMP_AMOUNT_PER_SHOT;
  cyclesFromLastWatering = 0;
}

const char *Pump::getName() {
  return name;
}

void Pump::cycle(bool cronMatches) {
  cyclesFromLastWatering++;
  if (cronMatches) {
    log(Debug, "  PMP: ON");
    on = true;
    cowLeft = cowPerShot - 1;
    cyclesFromLastWatering = 0;
  } else if (cowLeft != 0) {
    log(Debug, "  PMP: ON (STILL)", (int)cowLeft);
    on = true;
    cowLeft = constrainValue(cowLeft - 1, 0, MAX_WATER_PUMP_AMOUNT_PER_SHOT);
  } else {
    log(Debug, "  PMP: OFF");
    on = false;
  }
}

int Pump::getActuatorValue() {
  return on;
}

void Pump::setConfig(int configIndex, char *retroMsg, bool set) {
  switch (configIndex) {
    case (PumpConfigStateAmount):
      if (set) {
        cowPerShot =
            rollValue(cowPerShot + INCR_WATER_PUMP_AMOUNT_PER_SHOT, MIN_WATER_PUMP_AMOUNT_PER_SHOT, MAX_WATER_PUMP_AMOUNT_PER_SHOT);
      }
      sprintf(retroMsg, "%s %ds", MSG_PUMP_CONFIG_AMOUNT, cowPerShot);
      break;
    default:
      break;
  }
}

int Pump::getNroConfigs() {
  return (int)PumpConfigStateDelimiter;
}

void Pump::getInfo(int infoIndex, char *retroMsg) {
  switch (infoIndex) {
    case (PumpLastWatered):
      sprintf(retroMsg, "%s %02dh(cyc)", MSG_PUMP_INFO_LAST_WATERING, (int)(cyclesFromLastWatering / 3600));
      break;
  }
}

int Pump::getNroInfos() {
  return (int)PumpInfoDelimiter;
}
