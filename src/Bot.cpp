#include <Bot.h>
#include <Messages.h>

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
  auxStateIndex = 0;
  auxSubstateIndex = 0;
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

void Bot::setState(BotState s) { state = s; }

int Bot::getState() { return state; }

int Bot::getAuxStateIndex() { return auxStateIndex; }

int Bot::getAuxSubstateIndex() { return auxSubstateIndex; }

// PRIVATE

void Bot::toWelcomeState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) { stdOutWriteString(data.lcdMessage, ""); }

void Bot::toRunState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  char lcdUp[16 + 1];
  char lcdDown[16 + 1];
  if (timerInterrupt) {
    for (int aIndex = 0; aIndex < nroActors; aIndex++) {
      bool match = clock->matches(aIndex);
      actors[aIndex]->cycle(match);
    }
  } else if (setPressed) {
    nextInfoState();
  }
  updateInfo(lcdUp, lcdDown);
  stdOutWriteString(lcdUp, lcdDown);
}

void Bot::toConfigActorsState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  char lcdUp[16 + 1];
  char lcdDown[16 + 1];
  if (modePressed) {
    nextActorConfigState();
    if (!canChangeMode) { // not yet done with actors configuration
      sprintf(lcdUp, "%s %s", data.lcdMessage, actors[auxStateIndex]->getActorName());
      actors[auxStateIndex]->setConfig(auxSubstateIndex, lcdDown, DO_NOT_CHANGE);
    } else { // done with actors configuration
      sprintf(lcdUp, "%s", data.lcdMessage);
      sprintf(lcdDown, MSG_BOT_DONE_CONFIGURING_ACTORS);
    }
  } else if (setPressed && !canChangeMode) { // set pressed and not done with actors
    sprintf(lcdUp, "%s %s", data.lcdMessage, actors[auxStateIndex]->getActorName());
    actors[auxStateIndex]->setConfig(auxSubstateIndex, lcdDown, DO_CHANGE);
  }
  if (modePressed || setPressed) {
    stdOutWriteString(lcdUp, lcdDown);
  }
}

void Bot::toConfigFrequencyState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  char lcdDown[16 + 1];
  if (modePressed) {
    if (canChangeMode) { // just arrived to the config state
      canChangeMode = false;
      auxStateIndex = 0;
    } else {
      auxStateIndex++;
      if (auxStateIndex == nroActors) { // no more actors
        canChangeMode = true;
        auxStateIndex = 0;
      }
    }
  } else if (setPressed && !canChangeMode) {
    clock->setNextFrequency(auxStateIndex);
  }
  if (modePressed || setPressed) {
    if (canChangeMode) {
      sprintf(lcdDown, MSG_BOT_DONE_CONFIGURING_FREQUENCIES);
    } else {
      sprintf(lcdDown, "%s: %s", actors[auxStateIndex]->getActorName(), clock->getFrequencyDescription(auxStateIndex));
    }
    stdOutWriteString(data.lcdMessage, lcdDown);
  }
}

void Bot::toConfigHourState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  if (setPressed) {
    clock->increaseHour();
  }
  if (modePressed || setPressed) {
    char lcdDown[16 + 1];
    clock->getTimeAsString(lcdDown);
    stdOutWriteString(data.lcdMessage, lcdDown);
  }
}

void Bot::toConfigMinuteState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  if (setPressed) {
    clock->increaseMinute();
  }
  if (modePressed || setPressed) {
    char lcdDown[16 + 1];
    clock->getTimeAsString(lcdDown);
    stdOutWriteString(data.lcdMessage, lcdDown);
  }
}

void Bot::nextActorConfigState() {
  if (canChangeMode) { // just arrived to the config actors state
    canChangeMode = false;
    auxStateIndex = 0;
    auxSubstateIndex = 0;
  } else { // were here from previous cycle
    int nroActorConfigs = actors[auxStateIndex]->getNroConfigs();
    auxSubstateIndex++;
    if (auxSubstateIndex == nroActorConfigs) {
      auxStateIndex++;
      auxSubstateIndex = 0;
      if (auxStateIndex == nroActors) {
        canChangeMode = true;
      } // done with actors configuration
    }
  }
}
void Bot::updateInfo(char* lcdUp, char* lcdDown) {
  if (auxStateIndex < nroActors) { // infos for actors
    int nroActorInfoStates = actors[auxStateIndex]->getNroInfos();
    sprintf(lcdUp, "%s %s", MSG_BOT_RUN_STATE, actors[auxStateIndex]->getActorName()); // LCDUP: RUN ACTOR0
    if (auxSubstateIndex < nroActorInfoStates) { // actor infos
      actors[auxStateIndex]->getInfo(auxSubstateIndex, lcdDown);
    } else if (auxSubstateIndex == nroActorInfoStates) { // frequency infos
      sprintf(lcdDown, "%s %s", MSG_BOT_FREQUENCY_SET, clock->getFrequencyDescription(auxStateIndex));
    }
  } else if (auxStateIndex == nroActors) { // general infos
    switch (auxSubstateIndex) {
      case ClockInfo:
        sprintf(lcdUp, "%s %s", MSG_BOT_RUN_STATE, MSG_BOT_CLOCK);
        clock->getTimeAsString(lcdDown);
        break;
      default:
        break;
    }
  }
}

void Bot::nextInfoState() {
  if (auxStateIndex < nroActors) { // infos for actors
    int nroActorInfoStates = actors[auxStateIndex]->getNroInfos();
    auxSubstateIndex++;
    if (auxSubstateIndex == nroActorInfoStates + 1) {
      auxStateIndex++;
      auxSubstateIndex = 0;
    }
  } else if (auxStateIndex == nroActors) { // general infos
    auxSubstateIndex++;
    if (auxSubstateIndex == DelimiterBotInfo) {
      auxStateIndex++;
      auxSubstateIndex = 0;
    }
  }
  if (auxStateIndex == nroActors + 1) { // reset
    auxStateIndex = 0;
    auxSubstateIndex = 0;
  }
}
