#ifndef TEST_ACTOR_INC
#define TEST_ACTOR_INC

#include <Log.h>
#include <Misc.h>
#include <Actor.h>

enum TestActorConfigState { TestActorConfigStateAmount = 0, TestActorConfigStateAmount2, TestActorConfigStateDelimiter };

enum TestActorInfoState { TestActorInfoConfig = 0, TestActorLastWatered, TestActorInfoDelimiter };

class TestActor : public Actor {

private:
  const char *name;
  bool on;

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

#endif // TEST_ACTOR_INC
