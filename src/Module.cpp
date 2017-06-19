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

#include <Module.h>

#define CLASS "Module"
#define PUMP_ACTIVATION_TIME_OFFSET_UNIT MAX_WATER_PUMP_AMOUNT_PER_SHOT + 10 // to make the next
// pump start after the previous finished

#define SERVO_DEGREES_MINIM 5
#define SERVO_DEGREES_GAP 40
#define SERVO_DEGREES_PUMP0 (SERVO_DEGREES_MINIM + SERVO_DEGREES_GAP)
#define SERVO_DEGREES_PUMP1 (SERVO_DEGREES_PUMP0 + SERVO_DEGREES_GAP)
#define SERVO_DEGREES_PUMP2 (SERVO_DEGREES_PUMP1 + SERVO_DEGREES_GAP)
#define SERVO_DEGREES_PUMP3 (SERVO_DEGREES_PUMP2 + SERVO_DEGREES_GAP)


#define VALID_EEPROM_SIGNATURE 7784
#define VALID_EEPROM_SIGNATURE_ADDRESS 0
#define FACTOR_EEPROM_ADDRESS VALID_EEPROM_SIGNATURE_ADDRESS + sizeof(int)
#define PUMP0_EEPROM_ADDRESS FACTOR_EEPROM_ADDRESS + sizeof(float)
#define PUMP1_EEPROM_ADDRESS PUMP0_EEPROM_ADDRESS + sizeof(Pump)
#define PUMP2_EEPROM_ADDRESS PUMP1_EEPROM_ADDRESS + sizeof(Pump)
#define PUMP3_EEPROM_ADDRESS PUMP2_EEPROM_ADDRESS + sizeof(Pump)
#define LEVEL_EEPROM_ADDRESS PUMP3_EEPROM_ADDRESS + sizeof(Pump)

#define SERVO_CONTROL_DELAY_MS_TEST 200


Module::Module() {

  this->amountOfActors = 5;
  this->actors = new Actor *[amountOfActors + 1];

  // Firing order: pump 0, pump 2, pump 3, pump 1
  this->p0 = new Pump(MSG_PUMP_NAME0);
  this->p0->setOnValue(SERVO_DEGREES_PUMP0);
  this->pump0 = new Delayer(PUMP_ACTIVATION_TIME_OFFSET_UNIT * 0); // firing order; 0
  this->pump0->setActor(p0);
  this->actors[0] = pump0;

  this->p1 = new Pump(MSG_PUMP_NAME1);
  this->p1->setOnValue(SERVO_DEGREES_PUMP1);
  this->pump1 = new Delayer(PUMP_ACTIVATION_TIME_OFFSET_UNIT * 3); // firing order: 3
  this->pump1->setActor(p1);
  this->actors[1] = pump1;

  this->p2 = new Pump(MSG_PUMP_NAME2);
  this->p2->setOnValue(SERVO_DEGREES_PUMP2);
  this->pump2 = new Delayer(PUMP_ACTIVATION_TIME_OFFSET_UNIT * 1); // firing order: 1
  this->pump2->setActor(p2);
  this->actors[2] = pump2;

  this->p3 = new Pump(MSG_PUMP_NAME3);
  this->p3->setOnValue(SERVO_DEGREES_PUMP3);
  this->pump3 = new Delayer(PUMP_ACTIVATION_TIME_OFFSET_UNIT * 2); // firing order: 2
  this->pump3->setActor(p3);
  this->actors[3] = pump3;

  this->level = new Level(MSG_LEVEL_NAME);
  this->actors[4] = level;

  this->actors[5] = NULL; // end of array

  this->clock = new Clock(actors, amountOfActors);

  this->amountOfConfigurables = amountOfActors + 1;
  this->configurables = new Configurable *[amountOfConfigurables + 1];
  this->configurables[0] = clock;
  this->configurables[1] = pump0;
  this->configurables[2] = pump1;
  this->configurables[3] = pump2;
  this->configurables[4] = pump3;
  this->configurables[5] = level;
  this->configurables[6] = NULL; // end of array

  this->bot = new Bot(clock, actors, configurables);
  previousMode = (BotMode)bot->getMode();

  this->subCycle = 0;

  stdOutWriteStringFcn = NULL;
}

void Module::loop(bool mode, bool set, bool wdtWasTriggered) {

  bool justMovedToRunMode = (previousMode == ConfigConfigurablesMode && bot->getMode() == RunMode);

  if (mode || set) {
    digitalWrite(YELLOW_LED_PIN, HIGH); // haptic feedback led
  }

  TimingInterrupt interruptType = processInterruptType(wdtWasTriggered);
  log(CLASS, Info, "\n\n\nLOOP");

  // execute a cycle on the bot
  bot->cycle(mode, set, interruptType, ((float)this->subCycle) / SUB_CYCLES_PER_CYCLE);


  if (interruptType == TimingInterruptCycle) { // cycles (~1 second)
    loopAnyModeCycle();
  }

  if (bot->getMode() == ConfigConfigurablesMode && interruptType == TimingInterruptCycle) { // cycles (~1 second)
    loopConfigModeCycle();
  }

  if (bot->getMode() == RunMode) {
    if (interruptType == TimingInterruptCycle) { // cycles (~1 second)
      loopRunModeCycle();
    }
  }

  if (mode || set) {
    clearErrorLogged();
    digitalWrite(YELLOW_LED_PIN, LOW);
  }

  if (justMovedToRunMode) {
    saveToEEPROM();
  }

  previousMode = (BotMode)bot->getMode();

}

void Module::setup() {
  loadFromEEPROM(); // Pointers to callbacks of loaded objects will be broken at this point. Must be reassigned right after.
}

void Module::setReadLevelFunction(int (*readLevel)()) {
  level->setReadLevelFunction(readLevel);
}

void Module::setDigitalWriteFunction(void (*digitalWriteFunction)(unsigned char pin, unsigned char value)) {
  digitalWrite = digitalWriteFunction;
}

void Module::setStdoutWriteFunction(void (*stdOutWriteStringFunction)(const char *, const char *)) {
  this->stdOutWriteStringFcn = stdOutWriteStringFunction;
  bot->setStdoutFunction(stdOutWriteStringFunction);
}

void Module::safeWriteStdout(const char * up, const char *down) {
  if (stdOutWriteStringFcn != NULL) {
    stdOutWriteStringFcn(up, down);
  }
}

void Module::setServoWriteFunction(void (*f)(int, int, bool)) {
  servoWrite = f;
}


void Module::setFactor(float f) {
  clock->setFactor(f);
}

void Module::controlActuator(int actuatorValue, int pin) {
  if (actuatorValue && bot->getMode() == RunMode) {
    digitalWrite(pin, HIGH);
  } else {
    digitalWrite(pin, LOW);
  }
}

Bot *Module::getBot() {
  return bot;
}

Clock *Module::getClock() {
  return bot->getClock();
}

Level *Module::getLevel() {
  return level;
}

void Module::saveToEEPROM() {
#ifndef UNIT_TEST
  safeWriteStdout("EEPROM", "Saving...");
  // Factor
  float clockFactor = getClock()->getFactor();
  EEPROM.put(FACTOR_EEPROM_ADDRESS, clockFactor);
  // Pumps
  Pump pumpToStore = *p0;
  EEPROM.put(PUMP0_EEPROM_ADDRESS, pumpToStore);
  pumpToStore = *p1;
  EEPROM.put(PUMP1_EEPROM_ADDRESS, pumpToStore);
  pumpToStore = *p2;
  EEPROM.put(PUMP2_EEPROM_ADDRESS, pumpToStore);
  pumpToStore = *p3;
  EEPROM.put(PUMP3_EEPROM_ADDRESS, pumpToStore);
  // Level
  Level levelToStore = *getLevel();
  EEPROM.put(LEVEL_EEPROM_ADDRESS, levelToStore);

  EEPROM.put(VALID_EEPROM_SIGNATURE_ADDRESS, (int)VALID_EEPROM_SIGNATURE);
#endif // UNIT_TEST
}

void Module::loadFromEEPROM() {
#ifndef UNIT_TEST
   // Pointers to callbacks of loaded objects will be broken at this point. Must be reassigned right below.
  int eeepromSignature = 0;
  EEPROM.get(VALID_EEPROM_SIGNATURE_ADDRESS, eeepromSignature);
  if (eeepromSignature == VALID_EEPROM_SIGNATURE) { // Check for valid EEPROM content
    safeWriteStdout("EEPROM", "Loading...");
    // Factor
    float factor = 0.0f;
    EEPROM.get(FACTOR_EEPROM_ADDRESS, factor);
    setFactor(factor);

    // Pumps
    EEPROM.get(PUMP0_EEPROM_ADDRESS, p0);
    EEPROM.get(PUMP1_EEPROM_ADDRESS, p1);
    EEPROM.get(PUMP2_EEPROM_ADDRESS, p2);
    EEPROM.get(PUMP3_EEPROM_ADDRESS, p3);

    p0->setServoWriteFunction(servoWrite);
    p1->setServoWriteFunction(servoWrite);
    p2->setServoWriteFunction(servoWrite);
    p3->setServoWriteFunction(servoWrite);

    // Level
    EEPROM.get(LEVEL_EEPROM_ADDRESS, *getLevel());
  } else {
    safeWriteStdout("EEPROM", "Skipping...");
  }
#endif // UNIT_TEST
}

TimingInterrupt Module::processInterruptType(bool wdtWasTriggered) {
  TimingInterrupt interruptType = TimingInterruptNone;
  if (wdtWasTriggered) {
    subCycle = (subCycle + 1) % SUB_CYCLES_PER_CYCLE;
    interruptType = (subCycle == 0 ? TimingInterruptCycle : TimingInterruptSubCycle);
  }
  return interruptType;
}

void Module::loopAnyModeCycle() {
  bool onceIn2Cycles = (bot->getClock()->getSeconds() % 2) == 0;
  bool lcdLight = (bot->getMode() != RunMode) || isThereErrorLogged();
  controlActuator(level->getActuatorValue() && onceIn2Cycles, BUZZER_PIN);
  if (onceIn2Cycles && isThereErrorLogged()) {
    bot->stdOutWriteString(MSG_ERROR, getErrorLogged());
  }
  digitalWrite(LCD_A, lcdLight);
}

void Module::loopConfigModeCycle() {
  bool onceIn2Cycles = (bot->getClock()->getSeconds() % 5) == 0;
  if (onceIn2Cycles) {
    int ci = bot->getConfigurableIndex();
    switch(ci) {
      case 1: servoWrite(absolute(p0->getOnValue()), SERVO_CONTROL_DELAY_MS_TEST, false); break;
      case 2: servoWrite(absolute(p1->getOnValue()), SERVO_CONTROL_DELAY_MS_TEST, false); break;
      case 3: servoWrite(absolute(p2->getOnValue()), SERVO_CONTROL_DELAY_MS_TEST, false); break;
      case 4: servoWrite(absolute(p3->getOnValue()), SERVO_CONTROL_DELAY_MS_TEST, false); break;
      default: break;
    }
  }
}

void Module::loopRunModeCycle() {
  // no action
}

int Module::oneIfActive(int servoPos) {
  return (servoPos != 0 ? 1 : 0);
}

