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

#include <log4ino/Log.h>
#include <main4ino/Actor.h>
#include <main4ino/Misc.h>
#ifdef UNIT_TEST
#include <string.h>
#endif

#define MIN_WATER_PUMP_AMOUNT_PER_SHOT 1
#define DEFAULT_WATER_PUMP_AMOUNT_PER_SHOT 1
#define MAX_WATER_PUMP_AMOUNT_PER_SHOT 30
#define INCR_WATER_PUMP_AMOUNT_PER_SHOT 1

#define ON_VALUE_DISPERSER_RANGE_INC 4
#define ON_VALUE_DISPERSER_RANGE_MAX 20
#define ON_VALUE_DISPERSER_RANGE_MIN 0
#define ON_VALUE_DISPERSER_RANGE_DEFAULT 4
#define ON_VALUE_DISPERSER_INC 4

enum PumpConfigState {
  PumpConfigStateFrequency = 0,  // configuration of the frequency of the pump action
  PumpConfigStateAmount,         // configuration of the amount of water per shot (in seconds of pump on)
  PumpConfigStateVariationRange, // configuration of the amount of variation allowed when pump is on
  PumpConfigStateShoot,          // configuration state to shoot by real water (to let user measure the real amount)
  PumpConfigStateDelimiter       // delimiter of the configuration states
};

enum PumpInfoState {
  PumpLastWatered = 0, // info telling how long ago the last watering took place
  PumpWateringCount,   // info telling how many times the watering took place so far
  PumpInfoDelimiter
};

/**
* This actor aims to activate a pump (for instance for watering purposes).
* Shoots of water happen at a controlled frequency and have a controlled duration.
*/
class Pump : public Actor {

private:
  char name[NAME_LEN + 1];        // name of the current pump
  int onValue;                    // value getActuatorValue will return if the pump is activated.
  bool activated;                 // flag telling if the pump is activated or not
  int cowPerShot;                 // expressed in amount of cycles where the pump is on
  int cowLeft;                    // amount of Cycles Of Watering left where the pump should be on
  long cyclesFromLastWatering;    // amount of cycles when last watered
  int wateringCounter;            // counter of amount of waterings since the beginning of time
  int onValueDisperser;           // value disperser (helps making the value variate when pump is on)
  int onValueDisperserRange;      // value disperser range (to determine the minimum and maximum value variation when pump is on)
  bool onValueDisperserDirection; // value of the direction on which the variation is taking place (true increases)
  FreqConf freqConf;              // configuration of the frequency at which this actor will get triggered

public:
  Pump(const char *name);

  const char *getName();

  void cycle(bool cronMatches);
  void subCycle(float subCycle);

  /**
   * The actuatorValue is positive if the pump is on. Otherwise negative or zero.
   * The magnitude (absolute value) can be used to indicate servo positioning for instance
   * while the pump is still off.
   */
  int getActuatorValue();

  int getNroConfigs();
  void setConfig(int configIndex, char *retroMsg, bool set);

  void getInfo(int infoIndex, char *retroMsg);
  int getNroInfos();

  void setOnValue(int newOnValue);

  FreqConf *getFrequencyConfiguration();
};

#endif // PUMP_INC
