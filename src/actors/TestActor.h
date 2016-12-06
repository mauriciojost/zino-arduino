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

#ifndef TEST_ACTOR_INC
#define TEST_ACTOR_INC

#include <Log.h>
#include <Misc.h>
#include <actors/Actor.h>

enum TestActorConfigState {
  TestActorConfigStateAmount = 0,
  TestActorConfigStateAmount2,
  TestActorConfigStateDelimiter
};

enum TestActorInfoState {
  TestActorInfoConfig = 0,
  TestActorLastWatered,
  TestActorInfoDelimiter
};

/**
* This actor is used for testing purposes.
*/
class TestActor : public Actor {

private:
  const char *name;
  bool on;

public:
  TestActor(const char *name);

  const char *getName();

  void cycle(bool cronMatches);
  void subCycle();
  int getActuatorValue();

  int getNroConfigs();
  void setConfig(int configIndex, char *retroMsg, bool set);

  void getInfo(int infoIndex, char *retroMsg);
  int getNroInfos();

  void clearActuatorValue();
};

#endif // TEST_ACTOR_INC
