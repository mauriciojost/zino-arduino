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
#define DEFAULT_ON_VALUE 1

Pump::Pump(const char *n) {
  name = n;
  activated = false;
  cowLeft = 0;
  cowPerShot = DEFAULT_WATER_PUMP_AMOUNT_PER_SHOT;
  cyclesFromLastWatering = 0;
  onValue = DEFAULT_ON_VALUE;
  onValueDisperser = 0;
  onValueDisperserRange = ON_VALUE_DISPERSER_RANGE_DEFAULT;
  onValueDisperserDirection = true;
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
      cowLeft = cowPerShot - 1;
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
}

void Pump::subCycle(float subCycle) {
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
  return activated ? onValue + onValueDisperser: 0;
}

void Pump::setConfig(int configIndex, char *retroMsg, bool set) {
  switch (configIndex) {
    case (PumpConfigStateAmount):
      if (set) {
        cowPerShot =
            rollValue(cowPerShot + INCR_WATER_PUMP_AMOUNT_PER_SHOT, MIN_WATER_PUMP_AMOUNT_PER_SHOT, MAX_WATER_PUMP_AMOUNT_PER_SHOT);
      }
      sprintf(retroMsg, "%s%ds", MSG_PUMP_CONFIG_AMOUNT, cowPerShot);
      break;
    case (PumpConfigStateVariationRange):
      if (set) {
        onValueDisperserRange =
            rollValue(onValueDisperserRange + ON_VALUE_DISPERSER_RANGE_INC, ON_VALUE_DISPERSER_RANGE_MIN, ON_VALUE_DISPERSER_RANGE_MAX);
      }
      sprintf(retroMsg, "%s%ds", MSG_PUMP_CONFIG_VALUE_RANGE, onValueDisperserRange);
      break;
    case (PumpConfigStateShoot):
      if (set) {
        cycle(true);
      }
      sprintf(retroMsg, "%s%s", MSG_PUMP_CONFIG_SAMPLE_SHOT_TEST, (activated ? MSG_YES : MSG_NO));
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
      sprintf(retroMsg, "%s%02dh(cyc)", MSG_PUMP_INFO_LAST_WATERING, (int)(cyclesFromLastWatering / 3600));
      break;
  }
}

int Pump::getNroInfos() {
  return (int)PumpInfoDelimiter;
}

void Pump::setOnValue(int newOnValue) {
  onValue = newOnValue;
}
