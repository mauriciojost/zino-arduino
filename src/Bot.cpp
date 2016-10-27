#include <Bot.h>

#define DO_CHANGE true
#define DO_NOT_CHANGE false

// PUBLIC

Bot::Bot(void (*wrSt)(const char *, const char *), Actor **actors, int nroActors) {

  this->stdOutWriteString = wrSt;
  this->nroActors = nroActors;
  this->actors = actors;

  this->clock = new Clock(nroActors);
  this->state = WelcomeState;
  this->changeModeEnabled = true;
  this->actorIndex = 0;
  this->actorConfigIndex = 0;
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

int Bot::getActorIndex() { return this->actorIndex; }

int Bot::getActorConfigIndex() { return this->actorConfigIndex; }

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
  if (modePressed) {
    if (this->changeModeEnabled) { // just arrived to the config actors state
      this->changeModeEnabled = false;
      this->actorIndex = 0;
      this->actorConfigIndex = 0;
    } else { // were here from previous cycle
      int nroActorStates = this->actors[this->actorIndex]->getNroConfigs();
      this->actorConfigIndex++;
      if (this->actorConfigIndex == nroActorStates) {
        this->actorIndex++;
        this->actorConfigIndex = 0;
        if (this->actorIndex == nroActors) {
          this->changeModeEnabled = true; // done with actors configuration
        }
      }
    }
    if (!this->changeModeEnabled) { // not yet done with actors configuration
      this->actors[this->actorIndex]->setConfig(this->actorConfigIndex, buffer, DO_NOT_CHANGE);
    } else {
      sprintf(buffer, "DONE ACTORS");
    }
  } else if (setPressed && !this->changeModeEnabled) { // set pressed and not done with actors
    this->actors[this->actorIndex]->setConfig(this->actorConfigIndex, buffer, DO_CHANGE);
  }
  if (modePressed || setPressed) {
    this->stdOutWriteString(data.lcdMessage, buffer);
  }
}

void Bot::toConfigFrequencyState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  char buffer[16 + 1];
  if (modePressed) {
    if (this->changeModeEnabled) { // just arrived to the config state
      this->changeModeEnabled = false;
      this->actorIndex = 0;
    } else {
      this->actorIndex++;
      if (this->actorIndex == this->nroActors) { // no more actors
        this->changeModeEnabled = true;
        this->actorIndex = 0;
      }
    }
  } else if (setPressed && !changeModeEnabled) {
    this->clock->setNextFrequency(this->actorIndex);
  }
  if (modePressed || setPressed) {
    if (this->changeModeEnabled) {
      sprintf(buffer, "DONE WITH FREQ");
    } else {
      sprintf(buffer, "FREQ %d: %s", this->actorIndex, this->clock->getFrequencyDescription(this->actorIndex));
    }
    this->stdOutWriteString(data.lcdMessage, buffer);
  }
}


void Bot::toConfigHourState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  if (setPressed) {
    this->clock->increaseHour();
  }
  if (modePressed || setPressed) {
    char buffer[16 + 1];
    this->clock->getTimeAsString(buffer);
    this->stdOutWriteString(data.lcdMessage, buffer);
  }
}

void Bot::toConfigMinuteState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  if (setPressed) {
    this->clock->increaseMinute();
  }
  if (modePressed || setPressed) {
    char buffer[16 + 1];
    this->clock->getTimeAsString(buffer);
    this->stdOutWriteString(data.lcdMessage, buffer);
  }
}

