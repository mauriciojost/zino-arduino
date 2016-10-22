#include <Bot.h>

// PUBLIC

Bot::Bot(void (*wrSt)(const char *, const char *), Pump** actors, int nroActors) {
  this->state = WelcomeState;
  this->stdOutWriteString = wrSt;
  this->actors = actors;
  this->nroActors = nroActors;
  this->changeModeEnabled = true;
  this->changeActorEnabled = true;
  this->actorIndex = 0;
}

void Bot::cycle(bool modePressed, bool setPressed, bool timerInterrupt) {
  log(Info, "->BOT CYCLE");
  if (timerInterrupt) {
    this->clock.cycle();
  }
  BotState nextState = this->state; // no changes by default
  if (modePressed && this->changeModeEnabled) {
    log(Info, "->NEXT ST: ", (int)nextState);
    nextState = this->statesData[this->state].nextState;
    this->state = nextState;
  } else {
    log(Info, "->SAME ST: ", (int)this->state);
  }
  (this->*statesData[nextState].currentStateFunction)(this->statesData[nextState], modePressed, setPressed, timerInterrupt);
}

// PRIVATE

void Bot::toWelcomeState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  this->stdOutWriteString(data.lcdMessage, "");
}

void Bot::toRunState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  char buffer[16];
  if (timerInterrupt) {
    bool match = this->clock.matches();
    for (int n=0; n<this->nroActors; n++) {
      this->actors[n]->cycle(match);
    }
  }
  sprintf(buffer, "%dd %02d:%02d:%02d", (int)this->clock.getDays(),
          (int)this->clock.getHours(), (int)this->clock.getMinutes(),
          (int)this->clock.getSeconds());
  this->stdOutWriteString(data.lcdMessage, buffer);
}

void Bot::toConfigPeriodState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  if (setPressed) {
    this->clock.setNextFrequency();
  }
  this->stdOutWriteString(data.lcdMessage, this->clock.getFrequencyDescription());
}

void Bot::toConfigActorsState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  char buffer[16];
  bool justLoadedActor = false;
  bool doneWithActors = false;

  log(Debug, "Config actors");
  if (this->changeModeEnabled) { // just arrived to the config actors state
    log(Debug, "First actor");
    this->changeModeEnabled = false;
    this->changeActorEnabled = false;
    justLoadedActor = true;
  }
  if (this->changeActorEnabled) { // change actor if requested in the previous cycle
    log(Debug, "Change actor");
    justLoadedActor = true;
    this->changeActorEnabled = false;
    this->actorIndex = rollValue(this->actorIndex + 1, 0, this->nroActors + 1);
    if (this->actorIndex == this->nroActors) { // no more actors
      log(Debug, "No more actors");
      this->changeModeEnabled = true;
      this->changeActorEnabled = false;
      this->actorIndex = 0;
      doneWithActors = true;
    }
  }
  if (doneWithActors) {
    log(Debug, "Done with actors");
    sprintf(buffer, "DONE ACTORS");
  } else if (modePressed) {
    log(Debug, "Mode pressed");
    if (this->actors[this->actorIndex]->hasNextConfigState(justLoadedActor)) {
      log(Debug, "Has next config state, go to next substate");
      this->actors[this->actorIndex]->nextConfigState(buffer);
    } else {
      log(Debug, "Does not have next config state, ask to go to next actor");
      this->changeActorEnabled = true;
      sprintf(buffer, "DONE %s", this->actors[this->actorIndex]->getName());
    }
  } else if (setPressed) {
    this->actors[this->actorIndex]->setConfig(buffer);
    log(Debug, "Set in submode");
  }
  this->stdOutWriteString(data.lcdMessage, buffer);
}

void Bot::toConfigHourState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  char buffer[16];
  if (setPressed) {
    this->clock.increaseHour();
  }
  this->clock.getTimeString(buffer);
  this->stdOutWriteString(data.lcdMessage, buffer);
}

void Bot::toConfigMinuteState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  char buffer[16];
  if (setPressed) {
    this->clock.increaseMinute();
  }
  this->clock.getTimeString(buffer);
  this->stdOutWriteString(data.lcdMessage, buffer);
}


