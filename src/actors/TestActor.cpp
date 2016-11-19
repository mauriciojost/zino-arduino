#include <actors/TestActor.h>

#define CLASS "TestActor"

TestActor::TestActor(const char *n) {
  name = n;
  on = false;
}

const char *TestActor::getName() {
  return name;
}

void TestActor::cycle(bool cronMatches) {
  if (cronMatches) {
    log(CLASS, Debug, "  TACT: ON");
    on = true;
  } else {
    log(CLASS, Debug, "  TACT: OFF");
    on = false;
  }
}

int TestActor::getActuatorValue() {
  return on;
}

void TestActor::setConfig(int configIndex, char *retroMsg, bool set) {
  switch (configIndex) {
    case (TestActorConfigStateAmount):
      log(CLASS, Debug, "  TACT CNF: 1");
      sprintf(retroMsg, "TA_CNF_1");
      break;
    case (TestActorConfigStateAmount2):
      log(CLASS, Debug, "  TACT CNF: 1");
      sprintf(retroMsg, "TA_CNF_2");
      break;
    default:
      break;
  }
}

int TestActor::getNroConfigs() {
  return (int)TestActorConfigStateDelimiter;
}

void TestActor::getInfo(int infoIndex, char *retroMsg) {
  switch (infoIndex) {
    case (TestActorInfoConfig):
      sprintf(retroMsg, "TA_INF_1");
      break;
    case (TestActorLastWatered):
      sprintf(retroMsg, "TA_INF_2");
      break;
    default:
      break;
  }
}

int TestActor::getNroInfos() {
  return (int)TestActorInfoDelimiter;
}

void TestActor::clearActuatorValue() {
  on = false;
}
