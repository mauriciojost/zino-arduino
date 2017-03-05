/*
 * ZINO: low-budget autonomous plant care system.
 *
 * Copyright 2016 Mauricio Jost
 * Contact: mauriciojostx@gmail.com
 *
 * This file is part of ZINO.
 *
 * ZINO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * ZINO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ZINO. If not, see <http://www.gnu.org/licenses/>.
 */

#include <actors/Pump.h>
#include <ui/Messages.h>

#define CLASS "Pump"

Pump::Pump(const char *n) {
  strncpy(name, n, NAME_LEN);
  activated = false;
  cowLeft = 0;
  cowPerShot = DEFAULT_WATER_PUMP_AMOUNT_PER_SHOT;
  cyclesFromLastWatering = 0;
  wateringCounter = 0;
  onValue = ON_VALUE_DEFAULT;
  onValueDisperser = 0;
  onValueDisperserRange = ON_VALUE_DISPERSER_RANGE_DEFAULT;
  onValueDisperserDirection = true;
  onValueSilentCycles = ON_VALUE_DEFAULT_SILENT_CYCLES;
  freqConf.setFrequency(Never);
}

const char *Pump::getName() {
  return name;
}

void Pump::cycle(bool cronMatches) {
  cyclesFromLastWatering++;
  if (cronMatches) {
    log(CLASS, Debug, "  PMP: ON");
    if (cowPerShot > 0) {
      activated = true;
      wateringCounter++;
      cowLeft = cowPerShot - 1 + onValueSilentCycles; // cowPerShot + silent cycles (the latter for servo positioning)
      cyclesFromLastWatering = 0;
    }
  } else if (cowLeft > 0) {
    log(CLASS, Debug, "  PMP: ON (STILL)", (int)cowLeft);
    activated = true;
    cowLeft = constrainValue(cowLeft - 1, 0, MAX_WATER_PUMP_AMOUNT_PER_SHOT);
  } else {
    log(CLASS, Debug, "  PMP: OFF");
    activated = false;
  }

  if (activated) {
    if (onValueDisperserDirection) {
      onValueDisperser = onValueDisperser + ON_VALUE_DISPERSER_INC;
      if (onValueDisperser >= onValueDisperserRange) {
        onValueDisperserDirection = false;
      }
    } else {
      onValueDisperser = onValueDisperser - ON_VALUE_DISPERSER_INC;
      if (onValueDisperser <= -onValueDisperserRange) {
        onValueDisperserDirection = true;
      }
    }
  }
}

int Pump::getActuatorValue() {
  if (activated) {
    if (cyclesFromLastWatering < onValueSilentCycles) {      // just got a match
      return -(onValue + onValueDisperser); // pump off during silent cycles
    } else {
      return onValue + onValueDisperser; // pump on for the rest of the cycles
    }
  } else {
    return 0;
  }
}

void Pump::setConfig(int configIndex, char *retroMsg, SetMode set, int* value) {
  switch (configIndex) {
    case (PumpConfigStateAmount):
      if (set == SetNext) {
        cowPerShot =
            rollValue(cowPerShot + INCR_WATER_PUMP_AMOUNT_PER_SHOT, MIN_WATER_PUMP_AMOUNT_PER_SHOT, MAX_WATER_PUMP_AMOUNT_PER_SHOT);
      }
      sprintf(retroMsg, "%s%ds", MSG_PUMP_CONFIG_AMOUNT, cowPerShot);
      break;
    case (PumpConfigStateVariationRange):
      if (set == SetNext) {
        onValueDisperserRange =
            rollValue(onValueDisperserRange + ON_VALUE_DISPERSER_RANGE_INC, ON_VALUE_DISPERSER_RANGE_MIN, ON_VALUE_DISPERSER_RANGE_MAX);
      }
      sprintf(retroMsg, "%s%ddeg", MSG_PUMP_CONFIG_VALUE_RANGE, onValueDisperserRange);
      break;
    case (PumpConfigOnValue):
      if (set == SetNext) {
        onValue = rollValue(onValue + ON_VALUE_INC, ON_VALUE_MIN, ON_VALUE_MAX);
      }
      sprintf(retroMsg, "%s%ddeg", MSG_PUMP_CONFIG_ON_VALUE, onValue);
      break;
    case (PumpConfigStateShoot):
      if (set == SetNext) {
        cycle(true);
      }
      sprintf(retroMsg, "%s%s", MSG_PUMP_CONFIG_SAMPLE_SHOT_TEST, (activated ? MSG_PUMP_CONFIG_SAMPLE_SHOT_TEST_YES : MSG_NO));
      break;
    case (PumpConfigStateFrequency):
      if (set == SetNext) {
        freqConf.setNextFrequency();
      }
      sprintf(retroMsg, "%s%s", MSG_FREQ, freqConf.getFrequencyDescription());
      break;
    default:
      retroMsg[0] = 0;
      break;
  }
}

int Pump::getNroConfigs() {
  if (freqConf.isActivated()) {
    return (int)PumpConfigStateDelimiter;
  } else {
    return ((int)PumpConfigStateFrequency + 1);
  }
}

void Pump::getInfo(int infoIndex, char *retroMsg) {
  switch (infoIndex) {
    case (PumpLastWatered):
      sprintf(retroMsg, "%s%02dh(cyc)", MSG_PUMP_INFO_LAST_WATERING, (int)(cyclesFromLastWatering / 3600));
      break;
    case (PumpWateringCount):
      sprintf(retroMsg, "%s%02d", MSG_PUMP_INFO_WATERING_COUNT, (int)wateringCounter);
      break;
  }
}

int Pump::getNroInfos() {
  if (freqConf.isActivated()) {
    return (int)PumpInfoDelimiter;
  } else {
    return 0;
  }
}

FreqConf *Pump::getFrequencyConfiguration() {
  return &freqConf;
}

void Pump::setOnValue(int newOnValue) {
  onValue = newOnValue;
}

void Pump::setOnValueSilentCycles(int newValue) {
  onValueSilentCycles = newValue;
}

int Pump::getOnValue() {
  return onValue;
}
