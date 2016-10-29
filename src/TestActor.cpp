#include <TestActor.h>

TestActor::TestActor(const char *n) {
  name = n;
  on = false;
}

const char *TestActor::getActorName() { return name; }

void TestActor::cycle(bool mustActNow) {
  if (mustActNow) {
    log(Debug, "  ACT: ON");
    on = true;
  } else {
    log(Debug, "  ACT: OFF");
    on = false;
  }
}

int TestActor::getActorState() { return on; }

void TestActor::setConfig(int configIndex, char *retroMsg, bool set) {
  switch (configIndex) {
  case (TestActorConfigStateAmount):
    if (set) {
      log(Debug, "  ACT CNF: 1");
      sprintf(retroMsg, "CNF 1 %s", name);
    }
    break;
  case (TestActorConfigStateAmount2):
    if (set) {
      log(Debug, "  ACT CNF: 1");
      sprintf(retroMsg, "CNF 2 %s", name);
    }
    break;
  default:
    break;
  }
}

int TestActor::getNroConfigs() { return (int)TestActorConfigStateDelimiter; }

void TestActor::getInfo(int infoIndex, char *retroMsg) {
  switch (infoIndex) {
  case (TestActorInfoConfig):
    sprintf(retroMsg, "INFO1 %s", name);
    break;
  case (TestActorLastWatered):
    sprintf(retroMsg, "INFO2 %s", name);
    break;
  default:
    break;
  }
}

int TestActor::getNroInfos() { return (int)TestActorInfoDelimiter; }
