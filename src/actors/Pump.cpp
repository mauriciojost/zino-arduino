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
#define MS_PER_SHOT 1000

Pump::Pump(const char *n): freqConf(Never, OncePerHour) {
  strncpy(name, n, NAME_LEN);
  cowPerShot = DEFAULT_WATER_PUMP_AMOUNT_PER_SHOT;
  cyclesFromLastWatering = 0;
  shotsCounter = 0;
  onValue = ON_VALUE_DEFAULT;
  onValueDisperserRange = ON_VALUE_DISPERSER_RANGE_DEFAULT;
  freqConf.setFrequency(Never);
}

const char *Pump::getName() {
  return name;
}

void Pump::cycle(bool cronMatches) {
  cyclesFromLastWatering++;
  if (cronMatches) {
    int posBase = onValue;
    int direction = true;
    int range = onValueDisperserRange;
    int posOffset = 0;
    servoWriteSafe(posBase, MS_PER_SHOT * 2, false);
    for (int t = 0; t < cowPerShot; t++) {
      posOffset = posOffset + (direction? ON_VALUE_DISPERSER_RANGE_INC: -ON_VALUE_DISPERSER_RANGE_INC);
      if (absolute(posOffset) >= range) direction = !direction;
      int pos = posBase + posOffset;
      servoWriteSafe(pos, MS_PER_SHOT, true);
      cyclesFromLastWatering = 0;
    }
    servoWriteSafe(posBase, MS_PER_SHOT, false);
    shotsCounter++;
  } else {
    // no match, no watering
  }

}

int Pump::getActuatorValue() {
  return 0;
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
      sprintf(retroMsg, "%s%02d", MSG_PUMP_INFO_WATERING_COUNT, (int)shotsCounter);
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

int Pump::getOnValue() {
  return onValue;
}

void Pump::setServoWriteFunction(void (*f)(int, int, bool)) {
  servoWrite = f;
}

void Pump::servoWriteSafe(int pos, int ms, bool on) {
  if (servoWrite != NULL) {
    servoWrite(pos, ms, on);
  } else {
    log(CLASS, Warn, "servoWrite==NUL");
  }
}

