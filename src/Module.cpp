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

#define SERVO_DEGREES_DANGLING 10
#define SERVO_DEGREES_PUMP0 (SERVO_DEGREES_DANGLING + 45)
#define SERVO_DEGREES_PUMP1 (SERVO_DEGREES_PUMP0 + 45)

#define SERVO_ACTIVATED true
#define SERVO_DEACTIVATED false

Module::Module() {

  this->amountOfActors = 3;
  this->actors = new Actor*[amountOfActors + 1];

  this->p0 = new Pump(MSG_PUMP_NAME0);
  this->p0->setOnValue(SERVO_DEGREES_PUMP0);
  this->pump0 = new Delayer(p0, PUMP_ACTIVATION_OFFSET_UNIT * 0);
  this->actors[0] = pump0;

  this->p1 = new Pump(MSG_PUMP_NAME1);
  this->p1->setOnValue(SERVO_DEGREES_PUMP1);
  this->pump1 = new Delayer(p1, PUMP_ACTIVATION_OFFSET_UNIT * 1);
  this->actors[1] = pump1;

  this->level = new Level(MSG_LEVEL_NAME);
  this->actors[2] = level;

  this->actors[3] = NULL; // end of array

  this->clock = new Clock(amountOfActors);

  this->amountOfConfigurables = amountOfActors + 1;
  this->configurables = new Configurable*[amountOfConfigurables + 1];
  this->configurables[0] = clock;
  this->configurables[1] = pump0;
  this->configurables[2] = pump1;
  this->configurables[3] = level;
  this->configurables[4] = NULL; // end of array

  this->bot = new Bot(clock, actors, configurables);

  this->lcd = new Lcd(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

  this->servo = new Servox(SERVO_PIN);
}

void Module::loop(bool mode, bool set, bool wdt) {

  log(CLASS, Info, "\n\n\nLOOP");

  // execute a cycle on the bot
  bot->cycle(mode, set, wdt);

  bool onceIn5Cycles = (bot->getClock()->getSeconds() % 5) == 0;
  log(CLASS, Debug, "1/5: ", onceIn5Cycles);

  digitalWrite(LCD_A,bot->getMode() != RunMode);
  //controlActuatorWithServo(pump0->getActuatorValue(), PUMP0_PIN);
  //controlActuatorWithServo(pump1->getActuatorValue(), PUMP1_PIN);

  if (bot->getMode() == RunMode) {
    if (pump0->getActuatorValue() != 0 || pump1->getActuatorValue() != 0 ) {
      servoControl(SERVO_ACTIVATED, pump0->getActuatorValue() + pump1->getActuatorValue()); // only one should be different than 0 because of delayers
      digitalWrite(PUMP0_PIN, HIGH);
    } else {
      servoControl(SERVO_DEACTIVATED, SERVO_DEGREES_DANGLING);
      digitalWrite(PUMP0_PIN, LOW);
    }
  }

  controlActuator(level->getActuatorValue() && onceIn5Cycles, LEVEL_BUZZER_PIN);

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

Lcd* Module::getLcd() {
  return lcd;
}

Bot* Module::getBot() {
  return bot;
}

Clock* Module::getClock() {
  return bot->getClock();
}

Servox* Module::getServo() {
  return servo;
}

