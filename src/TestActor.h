#ifndef DUMB_ACTOR_INC
#define DUMB_ACTOR_INC

#include <Log.h>
#include <Misc.h>
#include <Actor.h>

enum TestActorConfigState { TestActorConfigStateAmount = 0, TestActorConfigStateAmount2, TestActorConfigStateDelimiter };

enum TestActorInfoState { TestActorInfoConfig = 0, TestActorLastWatered, TestActorInfoDelimiter };

class TestActor : public Actor {

private:
  const char *name; // name of the actor
  bool on;          // flag telling if the actor is on or not

public:
  TestActor(const char *name);

  const char *getName();

  void cycle(bool cronMatches);
  int getActuatorValue();

  int getNroConfigs();
  void setConfig(int configIndex, char *retroMsg, bool set);

  void getInfo(int infoIndex, char *retroMsg);
  int getNroInfos();
};

#endif // DUMB_ACTOR_INC
