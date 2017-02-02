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

#ifndef MODULE_INC
#define MODULE_INC

#include <main4ino/Actor.h>
#include <main4ino/Configurable.h>
#include <actors/Pump.h>
#include <actors/Delayer.h>
#include <actors/Level.h>
#include <ui/Lcd.h>
#include <main4ino/Clock.h>
#include <main4ino/Bot.h>
#include <hardware/AppPins.h>
#include <hardware/Servox.h>
#include <ui/Messages.h>

/**
* This class represents the integration of all components (LCD, buttons, buzzer, etc).
*/
class Module {

private:
  int amountOfActors;
  Pump *p0;
  Delayer *pump0;
  Pump *p1;
  Delayer *pump1;
  Pump *p2;
  Delayer *pump2;
  Pump *p3;
  Delayer *pump3;
  Level *level;
  Actor **actors;
  Clock *clock;
  int amountOfConfigurables;
  Configurable **configurables;
  Bot *bot;
  Lcd *lcd;
  Servox *servo;

  unsigned char subCycle;
  void (*digitalWrite)(unsigned char pin, unsigned char value);
  void controlActuator(int actValue, int pin);
  void servoControl(bool on, int position);
  int oneIfActive(int servoPos);

public:
  Module();

  void loop(bool mode, bool set, bool wdt);

  void setup();
  void setDigitalWriteFunction(void (*digitalWriteFunction)(unsigned char pin, unsigned char value));
  void setReadLevelFunction(int (*readLevel)());
  void setStdoutWriteFunction(void (*stdOutWriteStringFunction)(const char *, const char *));
  void setFactor(float f);

  Lcd *getLcd();
  Bot *getBot();
  Clock *getClock();
  Servox *getServo();
};

#endif // MODULE_INC
