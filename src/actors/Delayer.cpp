#include <actors/Delayer.h>
#include <ui/Messages.h>

#define CLASS "Delayer"

Delayer::Delayer(Actor *a, int o) {
  actor = a;
  offset = o;
  matched = false;
  passTheMatchIn = 0;
}

const char *Delayer::getName() {
  return actor->getName();
}

void Delayer::cycle(bool cronMatches) {
  if (cronMatches) {
    matched = true;
    passTheMatchIn = offset;
  }

  if (matched) {
    passTheMatchIn--;
    log(CLASS, Debug, "  DLY: GO ON", passTheMatchIn);
  }

  if (matched && passTheMatchIn <= 0) {
    log(CLASS, Debug, "  DLY: GO NOW");
    matched = false;
    passTheMatchIn = 0;
    actor->cycle(true);
  } else {
    log(CLASS, Debug, "  DLY: OFF");
    actor->cycle(false);
  }
}

int Delayer::getActuatorValue() {
  return actor->getActuatorValue();
}

void Delayer::setConfig(int configIndex, char *retroMsg, bool set) {
  actor->setConfig(configIndex, retroMsg, set);
}

int Delayer::getNroConfigs() {
  return actor->getNroConfigs();
}

void Delayer::getInfo(int infoIndex, char *retroMsg) {
  actor->getInfo(infoIndex, retroMsg);
}

int Delayer::getNroInfos() {
  return actor->getNroInfos();
}
