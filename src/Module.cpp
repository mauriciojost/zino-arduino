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


#define VALID_EEPROM_SIGNATURE 'a'
#define VALID_EEPROM_SIGNATURE_ADDRESS 0

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

  stdOutWriteStringFcn = NULL;
  eepromSave = NULL;
  eepromRead = NULL;

}

void Module::loop(bool mode, bool set, bool wdtWasTriggered) {

  bool justMovedToRunMode = (previousMode == ConfigConfigurablesMode && bot->getMode() == RunMode);

  if (mode || set) {
    digitalWrite(YELLOW_LED_PIN, HIGH); // haptic feedback led
  }

  TimingInterrupt interruptType = processInterruptType(wdtWasTriggered);
  log(CLASS, Info, "\n\n\nLOOP");

  previousMode = (BotMode)bot->getMode();

  // execute a cycle on the bot
  bot->cycle(mode, set, interruptType);


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
    digitalWrite(YELLOW_LED_PIN, LOW);
  }

  if (justMovedToRunMode) {
    saveToEEPROM();
  }

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

void Module::setServoWriteFunction(void (*f)(int, int, bool, bool)) {
  servoWrite = f;
  p0->setServoWriteFunction(servoWrite);
  p1->setServoWriteFunction(servoWrite);
  p2->setServoWriteFunction(servoWrite);
  p3->setServoWriteFunction(servoWrite);
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

TimingInterrupt Module::processInterruptType(bool wdtWasTriggered) {
  TimingInterrupt interruptType = TimingInterruptNone;
  if (wdtWasTriggered) {
    interruptType = TimingInterruptCycle;
  }
  return interruptType;
}

void Module::loopAnyModeCycle() {
  bool onceIn2Cycles = (bot->getClock()->getSeconds() % 2) == 0;
  bool lcdLight = (bot->getMode() != RunMode);
  controlActuator(level->getActuatorValue() && onceIn2Cycles, BUZZER_PIN);
  digitalWrite(LCD_A, lcdLight);
}

void Module::loopConfigModeCycle() {
  bool onceIn2Cycles = (bot->getClock()->getSeconds() % 5) == 0;
  if (onceIn2Cycles) {
    int ci = bot->getConfigurableIndex();
    switch(ci) {
      case 1: servoWrite(absolute(p0->getOnValue()), SERVO_CONTROL_DELAY_MS_TEST, false, true); break;
      case 2: servoWrite(absolute(p1->getOnValue()), SERVO_CONTROL_DELAY_MS_TEST, false, true); break;
      case 3: servoWrite(absolute(p2->getOnValue()), SERVO_CONTROL_DELAY_MS_TEST, false, true); break;
      case 4: servoWrite(absolute(p3->getOnValue()), SERVO_CONTROL_DELAY_MS_TEST, false, true); break;
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

void Module::saveToEEPROM() {
  char eepromSignature = VALID_EEPROM_SIGNATURE;
  int pos = VALID_EEPROM_SIGNATURE_ADDRESS;
  log(CLASS, Info, "EEP Save...");
  pos += sizeof(eepromSignature); p0->load(pos, eepromRead);
  pos += p0->saveSize(); p1->load(pos, eepromRead);
  pos += p1->saveSize(); p2->load(pos, eepromRead);
  pos += p2->saveSize(); p3->load(pos, eepromRead);
  eepromSave(VALID_EEPROM_SIGNATURE_ADDRESS, eepromSignature);
}

void Module::loadFromEEPROM() {
  char eepromSignature = eepromRead(VALID_EEPROM_SIGNATURE_ADDRESS);
  int pos = VALID_EEPROM_SIGNATURE_ADDRESS;

  if (eepromSignature != VALID_EEPROM_SIGNATURE) { // Check for valid EEPROM content
    log(CLASS, Warn, "EEP Skip...");
    return;
  }

  log(CLASS, Info, "EEP Load...");
  pos += sizeof(eepromSignature); p0->save(pos, eepromSave);
  pos += p0->saveSize(); p1->save(pos, eepromSave);
  pos += p1->saveSize(); p2->save(pos, eepromSave);
  pos += p2->saveSize(); p3->save(pos, eepromSave);
}

void Module::setEepromSave(void (*w)(int address, unsigned char byte)) {
  eepromSave = w;
}

void Module::setEepromRead(unsigned char(*r)(int address)) {
  eepromRead = r;
}
