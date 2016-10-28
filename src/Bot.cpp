#include <Bot.h>

#define DO_CHANGE true
#define DO_NOT_CHANGE false

// PUBLIC

Bot::Bot(void (*wrSt)(const char *, const char *), Actor **a, int nActors) {
  stdOutWriteString = wrSt;
  nroActors = nActors;
  actors = a;

  clock = new Clock(nroActors);
  state = WelcomeState;
  canChangeMode = true;
  actorIndex = 0;
  actorConfigIndex = 0;
}

void Bot::cycle(bool modePressed, bool setPressed, bool timerInterrupt) {
  if (timerInterrupt) {
    clock->cycle();
  }
  BotState nextState = state; // no changes by default
  if (modePressed && canChangeMode) {
    nextState = statesData[state].nextState;
    state = nextState;
    log(Info, "->(NEXT) ST: ", (int)nextState);
  } else {
    log(Info, "->(SAME) ST: ", (int)state);
  }
  (this->*statesData[nextState].currentStateFunction)(statesData[nextState], modePressed, setPressed, timerInterrupt);
}

int Bot::getState() { return state; }

int Bot::getActorIndex() { return actorIndex; }

int Bot::getActorConfigIndex() { return actorConfigIndex; }

// PRIVATE

void Bot::toWelcomeState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) { stdOutWriteString(data.lcdMessage, ""); }

void Bot::toRunState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  char buffer[16 + 1];
  if (timerInterrupt) {
    for (int aIndex = 0; aIndex < nroActors; aIndex++) {
      bool match = clock->matches(aIndex);
      actors[aIndex]->cycle(match);
    }
  }
  sprintf(buffer, "%dd %02d:%02d:%02d", clock->getDays(), clock->getHours(), clock->getMinutes(), clock->getSeconds());
  stdOutWriteString(data.lcdMessage, buffer);
}

void Bot::toConfigActorsState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  char buffer[16 + 1];
  if (modePressed) {
    if (canChangeMode) { // just arrived to the config actors state
      canChangeMode = false;
      actorIndex = 0;
      actorConfigIndex = 0;
    } else { // were here from previous cycle
      int nroActorConfigs = actors[actorIndex]->getNroConfigs();
      actorConfigIndex++;
      if (actorConfigIndex == nroActorConfigs) {
        actorIndex++;
        actorConfigIndex = 0;
        if (actorIndex == nroActors) {
          canChangeMode = true;
        } // done with actors configuration
      }
    }
    if (!canChangeMode) { // not yet done with actors configuration
      actors[actorIndex]->setConfig(actorConfigIndex, buffer, DO_NOT_CHANGE);
    } else {
      sprintf(buffer, "DONE ACTORS");
    }
  } else if (setPressed && !canChangeMode) { // set pressed and not done with actors
    actors[actorIndex]->setConfig(actorConfigIndex, buffer, DO_CHANGE);
  }
  if (modePressed || setPressed) {
    stdOutWriteString(data.lcdMessage, buffer);
  }
}

void Bot::toConfigFrequencyState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  char buffer[16 + 1];
  if (modePressed) {
    if (canChangeMode) { // just arrived to the config state
      canChangeMode = false;
      actorIndex = 0;
    } else {
      actorIndex++;
      if (actorIndex == nroActors) { // no more actors
        canChangeMode = true;
        actorIndex = 0;
      }
    }
  } else if (setPressed && !canChangeMode) {
    clock->setNextFrequency(actorIndex);
  }
  if (modePressed || setPressed) {
    if (canChangeMode) {
      sprintf(buffer, "DONE WITH FREQ");
    } else {
      sprintf(buffer, "FREQ %d: %s", actorIndex, clock->getFrequencyDescription(actorIndex));
    }
    stdOutWriteString(data.lcdMessage, buffer);
  }
}

void Bot::toConfigHourState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  if (setPressed) {
    clock->increaseHour();
  }
  if (modePressed || setPressed) {
    char buffer[16 + 1];
    clock->getTimeAsString(buffer);
    stdOutWriteString(data.lcdMessage, buffer);
  }
}

void Bot::toConfigMinuteState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  if (setPressed) {
    clock->increaseMinute();
  }
  if (modePressed || setPressed) {
    char buffer[16 + 1];
    clock->getTimeAsString(buffer);
    stdOutWriteString(data.lcdMessage, buffer);
  }
}
