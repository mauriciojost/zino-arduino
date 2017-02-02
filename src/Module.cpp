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
#define PUMP_ACTIVATION_OFFSET_UNIT 60

#define SERVO_DEGREES_DANGLING 5
#define SERVO_DEGREES_GAP 40
#define SERVO_DEGREES_PUMP0 (SERVO_DEGREES_DANGLING + SERVO_DEGREES_GAP)
#define SERVO_DEGREES_PUMP1 (SERVO_DEGREES_PUMP0 + SERVO_DEGREES_GAP)
#define SERVO_DEGREES_PUMP2 (SERVO_DEGREES_PUMP1 + SERVO_DEGREES_GAP)
#define SERVO_DEGREES_PUMP3 (SERVO_DEGREES_PUMP2 + SERVO_DEGREES_GAP)
#define SERVO_DEGREES_PARKING_INC 15

#define SERVO_ACTIVATED true
#define SERVO_DEACTIVATED false

Module::Module() {

  this->amountOfActors = 5;
  this->actors = new Actor *[amountOfActors + 1];

  this->p0 = new Pump(MSG_PUMP_NAME0);
  this->p0->setOnValue(SERVO_DEGREES_PUMP0);
  this->pump0 = new Delayer(p0, PUMP_ACTIVATION_OFFSET_UNIT * 0);
  this->actors[0] = pump0;

  this->p1 = new Pump(MSG_PUMP_NAME1);
  this->p1->setOnValue(SERVO_DEGREES_PUMP1);
  this->pump1 = new Delayer(p1, PUMP_ACTIVATION_OFFSET_UNIT * 1);
  this->actors[1] = pump1;

  this->p2 = new Pump(MSG_PUMP_NAME2);
  this->p2->setOnValue(SERVO_DEGREES_PUMP2);
  this->pump2 = new Delayer(p2, PUMP_ACTIVATION_OFFSET_UNIT * 2);
  this->actors[2] = pump2;

  this->p3 = new Pump(MSG_PUMP_NAME3);
  this->p3->setOnValue(SERVO_DEGREES_PUMP3);
  this->pump3 = new Delayer(p3, PUMP_ACTIVATION_OFFSET_UNIT * 3);
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

int Module::oneIfActive(int servoPos) {
  return (servoPos != 0 ? 1 : 0);
}

void Module::loop(bool mode, bool set, bool wdtWasTriggered) {

  bool anyButtonPressed = mode || set;
  TimingInterrupt interruptType = TimingInterruptNone;

  if (wdtWasTriggered) {
    subCycle = (subCycle + 1) % SUB_CYCLES_PER_CYCLE;
    if (subCycle == 0) {
      interruptType = TimingInterruptCycle;
    } else {
      interruptType = TimingInterruptSubCycle;
    }
  }

  log(CLASS, Info, "\n\n\nLOOP");

  // execute a cycle on the bot
  bot->cycle(mode, set, interruptType, ((float)subCycle) / SUB_CYCLES_PER_CYCLE);

  if (interruptType == TimingInterruptCycle) { // cycles (~1 second)
    bool onceIn2Cycles = (bot->getClock()->getSeconds() % 2) == 0;
    bool lcdLight = (bot->getMode() != RunMode) || isThereErrorLogged();
    controlActuator(level->getActuatorValue() && onceIn2Cycles, LEVEL_BUZZER_PIN);
    if (isThereErrorLogged() && onceIn2Cycles) {
      bot->stdOutWriteString(MSG_ERROR, getErrorLogged());
    }
    digitalWrite(LCD_A, lcdLight);
  }

  if (interruptType != TimingInterruptNone) { // sub sycles (less than 1 second)
    if (bot->getMode() == RunMode) {

      int pumpValueSum =  // only one plant should be active, i.e different than 0 (because of delayers)
        pump0->getActuatorValue() +
        pump1->getActuatorValue() +
        pump2->getActuatorValue() +
        pump3->getActuatorValue();

      if (pumpValueSum != 0) {
        int pumpsActive =
          oneIfActive(pump0->getActuatorValue()) +
          oneIfActive(pump1->getActuatorValue()) +
          oneIfActive(pump2->getActuatorValue()) +
          oneIfActive(pump3->getActuatorValue());

        if (pumpsActive > 1) {
          log(CLASS, Error, 1);
        } else {
          servoControl(pumpValueSum > 0, absolute(pumpValueSum)); // sends negative values if pump should not be on yet
          digitalWrite(PUMP_PIN, pumpValueSum > 0);
        }
      } else {
        digitalWrite(PUMP_PIN, LOW);
        if (servo->getLastPosition() <= SERVO_DEGREES_DANGLING) {
          servoControl(SERVO_DEACTIVATED, SERVO_DEGREES_DANGLING);
        } else {
          servoControl(SERVO_ACTIVATED, servo->getLastPosition() - SERVO_DEGREES_PARKING_INC); // it will decrease gradually until dangling range
        }
      }
    }
  }

  if (anyButtonPressed) {
    clearErrorLogged();
  }
}

void Module::setup() {
  lcd->initialize();
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
    servo->controlServo(true, position);
  } else {
    log(CLASS, Debug, "SRV: OFF");
    servo->controlServo(false, position);
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

