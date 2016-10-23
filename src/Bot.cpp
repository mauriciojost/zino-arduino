#include <Bot.h>

// PUBLIC

Bot::Bot(void (*wrSt)(const char *, const char *), Pump **actors, int nroActors) {

  this->stdOutWriteString = wrSt;
  this->nroActors = nroActors;
  this->actors = actors;

  this->clock = new Clock(nroActors);
  this->state = WelcomeState;
  this->changeModeEnabled = true;
  this->changeActorEnabled = true;
  this->actorConfigIndex = 0;
  this->clockFrequencyConfigIndex = 0;

}

void Bot::cycle(bool modePressed, bool setPressed, bool timerInterrupt) {
  if (timerInterrupt) {
    this->clock->cycle();
  }
  BotState nextState = this->state; // no changes by default
  if (modePressed && this->changeModeEnabled) {
    nextState = this->statesData[this->state].nextState;
    this->state = nextState;
    log(Info, "->(NEXT) ST: ", (int)nextState);
  } else {
    log(Info, "->(SAME) ST: ", (int)this->state);
  }
  (this->*statesData[nextState].currentStateFunction)(this->statesData[nextState], modePressed, setPressed, timerInterrupt);
}

int Bot::getState() { return this->state; }

int Bot::getActorIndex() { return this->actorConfigIndex; }

// PRIVATE

void Bot::toWelcomeState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) { this->stdOutWriteString(data.lcdMessage, ""); }

void Bot::toRunState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  char buffer[16 + 1];
  if (timerInterrupt) {
    for (int aIndex = 0; aIndex < this->nroActors; aIndex++) {
      bool match = this->clock->matches(aIndex);
      this->actors[aIndex]->cycle(match);
    }
  }
  sprintf(buffer, "%dd %02d:%02d:%02d", this->clock->getDays(), this->clock->getHours(), this->clock->getMinutes(), this->clock->getSeconds());
  this->stdOutWriteString(data.lcdMessage, buffer);
}

void Bot::toConfigActorsState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  char buffer[16 + 1];
  // * stands for actor
  // ** stands for actor state
  if (modePressed) {
    bool justLoadedConfigActorState = false;
    bool noMoreActors = false;

    if (this->changeModeEnabled) { // just arrived to the config actors state
      log(Debug, "1st*");
      this->changeModeEnabled = false;
      this->changeActorEnabled = false;
      justLoadedConfigActorState = true;
    } else if (this->changeActorEnabled) { // change actor if requested in the previous cycle
      log(Debug, "nex*");
      justLoadedConfigActorState = true;
      this->changeActorEnabled = false;
      this->actorConfigIndex = rollValue(this->actorConfigIndex + 1, 0, this->nroActors + 1);
      if (this->actorConfigIndex == this->nroActors) { // no more actors
        log(Debug, "no*");
        this->changeModeEnabled = true;
        this->changeActorEnabled = false;
        this->actorConfigIndex = 0;
        noMoreActors = true;
      }
    }

    if (noMoreActors) {
      sprintf(buffer, "DONE ACTORS");
    } else {
      if (this->actors[this->actorConfigIndex]->hasNextConfigState(justLoadedConfigActorState)) {
        log(Debug, "nex**");
        this->actors[this->actorConfigIndex]->nextConfigState(buffer);
      } else {
        log(Debug, "anex*");
        this->changeActorEnabled = true;
        sprintf(buffer, "DONE %s", this->actors[this->actorConfigIndex]->getName());
      }
    }
  } else if (setPressed) {
    this->actors[this->actorConfigIndex]->setConfig(buffer);
  }
  if (modePressed || setPressed) {
    this->stdOutWriteString(data.lcdMessage, buffer);
  }
}

void Bot::toConfigHourState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  if (setPressed) {
    this->clock->increaseHour();
  }
  if (modePressed || setPressed) {
    char buffer[16 + 1];
    this->clock->getTimeString(buffer);
    this->stdOutWriteString(data.lcdMessage, buffer);
  }
}

void Bot::toConfigMinuteState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  if (setPressed) {
    this->clock->increaseMinute();
  }
  if (modePressed || setPressed) {
    char buffer[16 + 1];
    this->clock->getTimeString(buffer);
    this->stdOutWriteString(data.lcdMessage, buffer);
  }
}

void Bot::toConfigFrequencyState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  char buffer[16 + 1];
  if (modePressed) {
    bool doneWithFrequencies = false;
    bool justLoaded = false;

    this->clockFrequencyConfigIndex = rollValue(this->clockFrequencyConfigIndex + 1, 0, this->nroActors + 1);
    if (this->changeModeEnabled) { // just arrived to the config state
      log(Debug, "1st**");
      this->changeModeEnabled = false;
      this->clockFrequencyConfigIndex = 0;
      justLoaded = true;
    }

    if (this->clockFrequencyConfigIndex == this->nroActors) { // no more actors
      doneWithFrequencies = true;
      this->changeModeEnabled = true;
    }
    if (doneWithFrequencies) {
      sprintf(buffer, "DONE WITH FREQ");
      this->changeModeEnabled = true;
      this->clockFrequencyConfigIndex = 0;
    } else {
      sprintf(buffer, "FREQ %d: %s", this->clockFrequencyConfigIndex, this->clock->getFrequencyDescription(this->clockFrequencyConfigIndex));
    }

  } else if (setPressed) {
    this->clock->setNextFrequency(this->clockFrequencyConfigIndex);
  }
  if (modePressed || setPressed) {
    this->stdOutWriteString(data.lcdMessage, buffer);
  }
}

