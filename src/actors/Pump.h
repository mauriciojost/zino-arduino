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
#define MAX_WATER_PUMP_AMOUNT_PER_SHOT 110
#define INCR_WATER_PUMP_AMOUNT_PER_SHOT 1

#define ON_VALUE_DEFAULT 0
#define ON_VALUE_INC 5
#define ON_VALUE_MAX 180
#define ON_VALUE_MIN 0

#define ON_VALUE_DEFAULT_SILENT_CYCLES 2

#define ON_VALUE_DISPERSER_RANGE_INC 2
#define ON_VALUE_DISPERSER_RANGE_MAX 40
#define ON_VALUE_DISPERSER_RANGE_MIN 0
#define ON_VALUE_DISPERSER_RANGE_DEFAULT 4
#define ON_VALUE_DISPERSER_INC 4


enum PumpConfigState {
  PumpConfigStateFrequency = 0,  // configuration of the frequency of the pump action
  PumpConfigStateAmount,         // configuration of the amount of water per shot (in seconds of pump on)
  PumpConfigOnValue,             // configuration of the on value (normally used to specify a servo position)
  PumpConfigStateVariationRange, // configuration of the amount of variation of the on value (normally used to move the servo while the pump is on)
  PumpConfigStateTestShoot,      // configuration state to shoot by real water (to let user measure the real amount)
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
  int cowPerShot;                 // expressed in amount of cycles where the pump is on
  bool testShot;                  // flag indicating if a test shot should be executed
  long cyclesFromLastWatering;    // amount of cycles when last watered
  int shotsCounter;               // counter of amount of waterings since boot
  int onValueDisperserRange;      // value disperser range (to determine the minimum and maximum value variation when pump is on)
  FreqConf freqConf;              // configuration of the frequency at which this actor will get triggered
  void (*servoWrite)(int pos, int ms, bool pump, bool smooth);

public:
  Pump(const char *name);

  const char *getName();

  void cycle(bool cronMatches);

  /**
   * The actuatorValue is positive if the pump is on. Otherwise negative or zero.
   * The magnitude (absolute value) can be used to indicate servo positioning for instance
   * while the pump is still off.
   */
  int getActuatorValue();

  int getNroConfigs();
  void setConfig(int configIndex, char *retroMsg, SetMode set, int* value = 0);

  void getInfo(int infoIndex, char *retroMsg);
  int getNroInfos();

  void setOnValue(int newOnValue);

  FreqConf *getFrequencyConfiguration();

  int getOnValue();

  void setServoWriteFunction(void (*f)(int, int, bool, bool));
  void servoWriteSafe(int pos, int ms, bool on, bool smooth);

};

#endif // PUMP_INC
