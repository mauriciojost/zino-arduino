#include <actors/Delayer.h>
#include <Messages.h>

Delayer::Delayer(Actor* a, int cowo) {
  actor = a;
  cowOffset = cowo;
  shouldHaveWatered = false;
  shouldHaveWateredAgo = 0;
}

const char *Delayer::getName() { return actor->getName(); }

void Delayer::cycle(bool cronMatches) {
  if (cronMatches) {
    shouldHaveWatered = true;
    shouldHaveWateredAgo = cowOffset;
  }

  if (shouldHaveWatered) {
    shouldHaveWateredAgo--;
    log(Debug, "  DLY: WILL ON", shouldHaveWateredAgo);
  }

  if (shouldHaveWatered && shouldHaveWateredAgo <= 0) {
    log(Debug, "  DLY: ON");
    shouldHaveWatered = false;
    shouldHaveWateredAgo = 0;
    actor->cycle(true);
  } else {
    log(Debug, "  DLY: OFF");
    actor->cycle(false);
  }

}

int Delayer::getActuatorValue() { return actor->getActuatorValue(); }

void Delayer::setConfig(int configIndex, char *retroMsg, bool set) {
    actor->setConfig(configIndex, retroMsg, set);
}

int Delayer::getNroConfigs() { return actor->getNroConfigs(); }

void Delayer::getInfo(int infoIndex, char *retroMsg) {
    actor->getInfo(infoIndex, retroMsg);
}

int Delayer::getNroInfos() { return actor->getNroInfos(); }
