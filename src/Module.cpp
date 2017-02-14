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
#define PUMP_ACTIVATION_TIME_OFFSET_UNIT 60

#define SERVO_DEGREES_MINIM 5
#define SERVO_DEGREES_GAP 40
#define SERVO_DEGREES_PUMP0 (SERVO_DEGREES_MINIM + SERVO_DEGREES_GAP)
#define SERVO_DEGREES_PUMP1 (SERVO_DEGREES_PUMP0 + SERVO_DEGREES_GAP)
#define SERVO_DEGREES_PUMP2 (SERVO_DEGREES_PUMP1 + SERVO_DEGREES_GAP)
#define SERVO_DEGREES_PUMP3 (SERVO_DEGREES_PUMP2 + SERVO_DEGREES_GAP)

#define SERVO_ACTIVATED true
#define SERVO_DEACTIVATED false

#define VALID_EEPROM_SIGNATURE 7781
#define VALID_EEPROM_SIGNATURE_ADDRESS 0
#define FACTOR_EEPROM_ADDRESS VALID_EEPROM_SIGNATURE_ADDRESS + sizeof(int)
#define PUMP0_EEPROM_ADDRESS FACTOR_EEPROM_ADDRESS + sizeof(float)
#define PUMP1_EEPROM_ADDRESS PUMP0_EEPROM_ADDRESS + sizeof(Pump)
#define PUMP2_EEPROM_ADDRESS PUMP1_EEPROM_ADDRESS + sizeof(Pump)
#define PUMP3_EEPROM_ADDRESS PUMP2_EEPROM_ADDRESS + sizeof(Pump)
#define LEVEL_EEPROM_ADDRESS PUMP3_EEPROM_ADDRESS + sizeof(Pump)


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

  this->lcd = new Lcd(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

  this->servo = new Servox(SERVO_PIN);

  this->subCycle = 0;
}

void Module::loop(bool mode, bool set, bool wdtWasTriggered) {

  TimingInterrupt interruptType = processInterruptType(wdtWasTriggered);
  log(CLASS, Info, "\n\n\nLOOP");

  // execute a cycle on the bot
  bot->cycle(mode, set, interruptType, ((float)this->subCycle) / SUB_CYCLES_PER_CYCLE);

  if (interruptType == TimingInterruptCycle) { // cycles (~1 second)
    loopAnyModeCycle();
  }

  if (bot->getMode() == RunMode && (interruptType == TimingInterruptCycle || interruptType == TimingInterruptSubCycle)) { // sub sycles (less than 1 second)
    loopRunModeSubCycle();
  }

  if (mode || set) {
    clearErrorLogged();
  }

  if (set) {
    saveToEEPROM();
  }

}

void Module::setup() {
  lcd->initialize();
  loadFromEEPROM(); // Pointers to callbacks of loaded objects will be broken at this point. Must be reassigned right after.
}

void Module::setReadLevelFunction(int (*readLevel)()) {
  level->setReadLevelFunction(readLevel);
}

void Module::setDigitalWriteFunction(void (*digitalWriteFunction)(unsigned char pin, unsigned char value)) {
  digitalWrite = digitalWriteFunction;
}

void Module::setStdoutWriteFunction(void (*stdOutWriteStringFunction)(const char *, const char *)) {
  bot->setStdoutFunction(stdOutWriteStringFunction);
}

void Module::servoControl(bool on, int position) {
  if (on) {
    log(CLASS, Debug, "SRV: ON");
    servo->controlServo(SERVO_ACTIVATED, position);
  } else {
    log(CLASS, Debug, "SRV: OFF");
    servo->controlServo(SERVO_DEACTIVATED, position);
  }
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

Lcd *Module::getLcd() {
  return lcd;
}

Bot *Module::getBot() {
  return bot;
}

Clock *Module::getClock() {
  return bot->getClock();
}

Servox *Module::getServo() {
  return servo;
}

Pump *Module::getPump0() {
  return p0;
}

Pump *Module::getPump1() {
  return p1;
}

Pump *Module::getPump2() {
  return p2;
}

Pump *Module::getPump3() {
  return p3;
}

Level *Module::getLevel() {
  return level;
}

void Module::saveToEEPROM() {
#ifndef UNIT_TEST
  log(CLASS, Debug, "EEP SAVE");
  // Factor
  float clockFactor = getClock()->getFactor();
  EEPROM.put(FACTOR_EEPROM_ADDRESS, clockFactor);
  // Pumps
  Pump pumpToStore = *getPump0();
  EEPROM.put(PUMP0_EEPROM_ADDRESS, pumpToStore);
  pumpToStore = *getPump1();
  EEPROM.put(PUMP1_EEPROM_ADDRESS, pumpToStore);
  pumpToStore = *getPump2();
  EEPROM.put(PUMP2_EEPROM_ADDRESS, pumpToStore);
  pumpToStore = *getPump3();
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
    log(CLASS, Debug, "EEP LOAD");
    // Factor
    float factor = 0.0f;
    EEPROM.get(FACTOR_EEPROM_ADDRESS, factor);
    setFactor(factor);

    // Pumps
    EEPROM.get(PUMP0_EEPROM_ADDRESS, *getPump0());
    EEPROM.get(PUMP1_EEPROM_ADDRESS, *getPump1());
    EEPROM.get(PUMP2_EEPROM_ADDRESS, *getPump2());
    EEPROM.get(PUMP3_EEPROM_ADDRESS, *getPump3());

    // Level
    EEPROM.get(LEVEL_EEPROM_ADDRESS, *getLevel());
  } else {
    log(CLASS, Warn, "EEP SKIP");
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
  controlActuator(level->getActuatorValue() && onceIn2Cycles, LEVEL_BUZZER_PIN);
  if (onceIn2Cycles) {
    bot->nextInfoState();
    if (isThereErrorLogged()) {
      bot->stdOutWriteString(MSG_ERROR, getErrorLogged());
    }
  }
  digitalWrite(LCD_A, lcdLight);
}

void Module::loopRunModeSubCycle() {

  int pumpsActive =  // only one plant should be active, i.e different than 0 (because of delayers)
    oneIfActive(pump0->getActuatorValue()) +
    oneIfActive(pump1->getActuatorValue()) +
    oneIfActive(pump2->getActuatorValue()) +
    oneIfActive(pump3->getActuatorValue());

  if (pumpsActive == 1) {
    int pumpValueSum =
      pump0->getActuatorValue() +
      pump1->getActuatorValue() +
      pump2->getActuatorValue() +
      pump3->getActuatorValue();
    servoControl(pumpValueSum > 0, absolute(pumpValueSum)); // sends negative values if pump should not be on yet
    digitalWrite(PUMP_PIN, pumpValueSum > 0);
  } else if (pumpsActive == 0) {
    digitalWrite(PUMP_PIN, LOW);
  } else {
    log(CLASS, Error, 1);
  }

}

int Module::oneIfActive(int servoPos) {
  return (servoPos != 0 ? 1 : 0);
}

