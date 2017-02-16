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

#include <actors/Delayer.h>
#include <ui/Messages.h>

#define CLASS "Delayer"

Delayer::Delayer(int o) {
  actor = NULL;
  offset = o;
  matched = false;
  passTheMatchIn = 0;
}

void Delayer::setActor(Actor* a) {
  actor = a;
}

const char *Delayer::getName() {
  return actor->getName();
}

void Delayer::cycle(bool cronMatches) {
  if (cronMatches) {
    matched = true;
    passTheMatchIn = offset;
  }

  if (matched) {
    passTheMatchIn--;
    log(CLASS, Debug, "  DLY: GO ON", passTheMatchIn);
  }

  if (matched && passTheMatchIn <= 0) {
    log(CLASS, Debug, "  DLY: GO NOW");
    matched = false;
    passTheMatchIn = 0;
    actor->cycle(true);
  } else {
    log(CLASS, Debug, "  DLY: OFF");
    actor->cycle(false);
  }
}

void Delayer::subCycle(float subCycle) {
  actor->subCycle(subCycle);
}

int Delayer::getActuatorValue() {
  return actor->getActuatorValue();
}

void Delayer::setConfig(int configIndex, char *retroMsg, SetMode set, int* value) {
  actor->setConfig(configIndex, retroMsg, set, value);
}

int Delayer::getNroConfigs() {
  return actor->getNroConfigs();
}

void Delayer::getInfo(int infoIndex, char *retroMsg) {
  actor->getInfo(infoIndex, retroMsg);
}

int Delayer::getNroInfos() {
  return actor->getNroInfos();
}

FreqConf *Delayer::getFrequencyConfiguration() {
  return actor->getFrequencyConfiguration();
}
