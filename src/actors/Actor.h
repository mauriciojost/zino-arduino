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

#ifndef ACTOR_INC
#define ACTOR_INC

#include <actors/Configurable.h>

/**
* This class specifies the interface of actors, components of the
* embedded system which:
* - can be configured via configuration states (via [[Configurable]])
* - can inform their state, via info states (via [[Configurable]])
* - can inform the actuator value (if any) (a LED, a pump, a buzzer)
*/
class Actor : public Configurable {

public:
  /**
  * Make the actor execute a cycle.
  */
  virtual void cycle(bool cronMatches) = 0;

  /**
  * Get the current actuator value.
  */
  virtual int getActuatorValue() = 0;
};

#endif // ACTOR_INC
