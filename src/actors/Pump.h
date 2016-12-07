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

#ifndef PUMP_INC
#define PUMP_INC

#include <Log.h>
#include <Misc.h>
#include <actors/Actor.h>

#define MIN_WATER_PUMP_AMOUNT_PER_SHOT 0
#define DEFAULT_WATER_PUMP_AMOUNT_PER_SHOT 0
#define MAX_WATER_PUMP_AMOUNT_PER_SHOT 30
#define INCR_WATER_PUMP_AMOUNT_PER_SHOT 1

enum PumpConfigState {
  PumpConfigStateAmount = 0, // configuration of the amount of water per shot (in seconds of pump on)
  PumpConfigStateShoot,      // configuration state to shoot by real water (to let user measure the real amount)
  PumpConfigStateDelimiter   // delimiter of the configuration states
};

enum PumpInfoState {
  PumpLastWatered = 0, // info telling how long ago the last watering took place
  PumpInfoDelimiter
};

/**
* This actor aims to activate a pump (for instance for watering purposes).
* Shoots of water happen at a controlled frequency and have a controlled duration.
*/
class Pump : public Actor {

private:
  const char *name;            // name of the current pump
  int onValue;                 // value getActuatorValue will return if the pump is activated.
  bool activated;              // flag telling if the pump is activated or not
  int cowPerShot;              // expressed in amount of cycles where the pump is on
  int cowLeft;                 // amount of Cycles Of Watering left where the pump should be on
  long cyclesFromLastWatering; // amount of cycles when last watered

public:
  Pump(const char *name);

  const char *getName();

  void cycle(bool cronMatches);
  void subCycle(float subCycle);
  int getActuatorValue();

  int getNroConfigs();
  void setConfig(int configIndex, char *retroMsg, bool set);

  void getInfo(int infoIndex, char *retroMsg);
  int getNroInfos();

  void setOnValue(int newOnValue);
};

#endif // PUMP_INC
