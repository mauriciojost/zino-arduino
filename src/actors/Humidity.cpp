#include <actors/Humidity.h>
#include <ui/Messages.h>

Humidity::Humidity(Actor *a, int o) {
  actor = a;
  offset = o;
  matched = false;
  passTheMatchIn = 0;
}

const char *Humidity::getName() {
  return actor->getName();
}

void Humidity::cycle(bool cronMatches) {
  if (cronMatches) {
    matched = true;
    passTheMatchIn = offset;
  }

  if (matched) {
    passTheMatchIn--;
    log(Debug, "  DLY: GO ON", passTheMatchIn);
  }

  if (matched && passTheMatchIn <= 0) {
    log(Debug, "  DLY: GO NOW");
    matched = false;
    passTheMatchIn = 0;
    actor->cycle(true);
  } else {
    log(Debug, "  DLY: OFF");
    actor->cycle(false);
  }
}

int Humidity::getActuatorValue() {
  return actor->getActuatorValue();
}

void Humidity::setConfig(int configIndex, char *retroMsg, bool set) {
  actor->setConfig(configIndex, retroMsg, set);
}

int Humidity::getNroConfigs() {
  return actor->getNroConfigs();
}

void Humidity::getInfo(int infoIndex, char *retroMsg) {
  actor->getInfo(infoIndex, retroMsg);
}

int Humidity::getNroInfos() {
  return actor->getNroInfos();
}
