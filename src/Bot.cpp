#include <Bot.h>

// PUBLIC

Bot::Bot(void (*wrSt)(const char *, const char *), Pump **actors, int nroActors) {
  this->state = WelcomeState;
  this->stdOutWriteString = wrSt;
  this->actors = actors;
  this->nroActors = nroActors;
  this->changeModeEnabled = true;
  this->changeActorEnabled = true;
  this->actorIndex = 0;
}

void Bot::cycle(bool modePressed, bool setPressed, bool timerInterrupt) {
  if (timerInterrupt) {
    this->clock.cycle();
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

// PRIVATE

void Bot::toWelcomeState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) { this->stdOutWriteString(data.lcdMessage, ""); }

void Bot::toRunState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  char buffer[16];
  if (timerInterrupt) {
    bool match = this->clock.matches();
    for (int n = 0; n < this->nroActors; n++) {
      this->actors[n]->cycle(match);
    }
  }
  sprintf(buffer, "%dd %02d:%02d:%02d", (int)this->clock.getDays(), (int)this->clock.getHours(), (int)this->clock.getMinutes(), (int)this->clock.getSeconds());
  this->stdOutWriteString(data.lcdMessage, buffer);
}

void Bot::toConfigPeriodState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  if (setPressed) {
    this->clock.setNextFrequency();
  }
  if (modePressed || setPressed) {
    this->stdOutWriteString(data.lcdMessage, this->clock.getFrequencyDescription());
  }
}

void Bot::toConfigActorsState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  char buffer[16];
  // * stands for actor
  // ** stands for actor state
  if (modePressed) {
    bool justLoadedActor = false;
    bool doneWithActors = false;

    if (this->changeModeEnabled) { // just arrived to the config actors state
      log(Debug, "1st*");
      this->changeModeEnabled = false;
      this->changeActorEnabled = false;
      justLoadedActor = true;
    } else if (this->changeActorEnabled) { // change actor if requested in the previous cycle
      log(Debug, "nex*");
      justLoadedActor = true;
      this->changeActorEnabled = false;
      this->actorIndex = rollValue(this->actorIndex + 1, 0, this->nroActors + 1);
      if (this->actorIndex == this->nroActors) { // no more actors
        log(Debug, "no*");
        this->changeModeEnabled = true;
        this->changeActorEnabled = false;
        this->actorIndex = 0;
        doneWithActors = true;
      }
    }
    if (doneWithActors) {
      sprintf(buffer, "DONE ACTORS");
    } else {
      if (this->actors[this->actorIndex]->hasNextConfigState(justLoadedActor)) {
        log(Debug, "nex**");
        this->actors[this->actorIndex]->nextConfigState(buffer);
      } else {
        log(Debug, "anex*");
        this->changeActorEnabled = true;
        sprintf(buffer, "DONE %s", this->actors[this->actorIndex]->getName());
      }
    }
  } else if (setPressed) {
    this->actors[this->actorIndex]->setConfig(buffer);
  }
  if (modePressed || setPressed) {
    this->stdOutWriteString(data.lcdMessage, buffer);
  }
}

void Bot::toConfigHourState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  if (setPressed) {
    this->clock.increaseHour();
  }
  if (modePressed || setPressed) {
    char buffer[16];
    this->clock.getTimeString(buffer);
    this->stdOutWriteString(data.lcdMessage, buffer);
  }
}

void Bot::toConfigMinuteState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  if (setPressed) {
    this->clock.increaseMinute();
  }
  if (modePressed || setPressed) {
    char buffer[16];
    this->clock.getTimeString(buffer);
    this->stdOutWriteString(data.lcdMessage, buffer);
  }
}
