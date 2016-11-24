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

#include <actors/Actor.h>
#include <actors/Configurable.h>
#include <actors/Pump.h>
#include <actors/Delayer.h>
#include <actors/Level.h>
#include <ui/Lcd.h>
#include <Clock.h>
#include <Bot.h>
#include <hardware/AppPins.h>

/**
* This class represents the integration of all components (LCD, buttons, buzzer, etc).
*/
class Module {

private:

  int amountOfActors;
  Pump* p0;
  Delayer* pump0;
  Pump* p1;
  Delayer* pump1;
  Level* level;
  Actor **actors;
  Clock* clock;
  int amountOfConfigurables;
  Configurable **configurables;
  Bot* bot;
  Lcd* lcd;
  void (*digitalWrite)(unsigned char pin, unsigned char value);

  void controlActuator(int aState, int pin);

public:
  Module();

  void loop(bool mode, bool set, bool wdt);

  void setup();
  void setDigitalWriteFunction(void (*digitalWriteFunction)(unsigned char pin, unsigned char value));
  void setReadLevelFunction(int (*readLevel)());
  void setStdoutWriteFunction(void (*stdOutWriteStringFunction)(const char *, const char *));
  void setFactor(float f);

  Lcd* getLcd();
  Bot* getBot();
  Clock* getClock();

};

#endif // MODULE_INC
