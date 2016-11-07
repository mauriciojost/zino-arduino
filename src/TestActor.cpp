#include <TestActor.h>

TestActor::TestActor(const char *n) {
  name = n;
  on = false;
}

const char *TestActor::getName() { return name; }

void TestActor::cycle(bool cronMatches) {
  if (cronMatches) {
    log(Debug, "  ACT: ON");
    on = true;
  } else {
    log(Debug, "  ACT: OFF");
    on = false;
  }
}

int TestActor::getActuatorValue() { return on; }

void TestActor::setConfig(int configIndex, char *retroMsg, bool set) {
  switch (configIndex) {
  case (TestActorConfigStateAmount):
    log(Debug, "  ACT CNF: 1");
    sprintf(retroMsg, "CNF 1");
    break;
  case (TestActorConfigStateAmount2):
    log(Debug, "  ACT CNF: 1");
    sprintf(retroMsg, "CNF 2");
    break;
  default:
    break;
  }
}

int TestActor::getNroConfigs() { return (int)TestActorConfigStateDelimiter; }

void TestActor::getInfo(int infoIndex, char *retroMsg) {
  switch (infoIndex) {
  case (TestActorInfoConfig):
    sprintf(retroMsg, "INFO1");
    break;
  case (TestActorLastWatered):
    sprintf(retroMsg, "INFO2");
    break;
  default:
    break;
  }
}

int TestActor::getNroInfos() { return (int)TestActorInfoDelimiter; }
