#ifndef DUMB_ACTOR_INC
#define DUMB_ACTOR_INC

#include <Log.h>
#include <Misc.h>
#include <Actor.h>

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

class TestActor : public Actor {

private:
  const char *name;         // name of the actor
  bool on;                  // flag telling if the actor is on or not

public:
  TestActor(const char *name);

  const char *getActorName();

  void cycle(bool mustActNow);
  int getActorState();

  int getNroConfigs();
  void setConfig(int configIndex, char *retroMsg, bool set);

  void getInfo(int infoIndex, char *retroMsg);
  int getNroInfos();
};

#endif // DUMB_ACTOR_INC
