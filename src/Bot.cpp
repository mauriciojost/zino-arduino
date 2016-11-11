#include <Bot.h>
#include <ui/Messages.h>

#define DO_CHANGE true
#define DO_NOT_CHANGE false

Bot::Bot(void (*wrSt)(const char *, const char *), Actor **a, int nActors) {
  stdOutWriteString = wrSt;
  nroActors = nActors;
  actors = a;

  clock = new Clock(nroActors, CYCLE_TO_SECONDS_FACTOR_DEFAULT);
  mode = WelcomeMode;
  canChangeMode = true;
  actorIndex = 0;
  actorStateIndex = 0;
}

void Bot::cycle(bool modePressed, bool setPressed, bool timerInterrupt) {
  if (timerInterrupt) {
    clock->cycle();
  }
  BotMode nextMode = mode; // no changes by default
  if (modePressed && canChangeMode) {
    nextMode = modesData[mode].nextMode;
    mode = nextMode;
    log(Info, "->(NEXT) ST: ", modesData[mode].lcdMessage);
  } else {
    log(Info, "->(SAME) ST: ", modesData[mode].lcdMessage);
  }
  (this->*modesData[nextMode].currentModeFunction)(&modesData[nextMode], modePressed, setPressed, timerInterrupt);
}

void Bot::setMode(BotMode s) {
  mode = s;
}

int Bot::getMode() {
  return mode;
}

int Bot::getActorIndex() {
  return actorIndex;
}

int Bot::getActorStateIndex() {
  return actorStateIndex;
}

void Bot::toWelcomeMode(BotModeData *data, bool modePressed, bool setPressed, bool timerInterrupt) {
  stdOutWriteString(data->lcdMessage, "");
}

void Bot::toRunMode(BotModeData *data, bool modePressed, bool setPressed, bool timerInterrupt) {
  char lcdUp[LCD_LENGTH + 1];
  char lcdDown[LCD_LENGTH + 1];
  if (timerInterrupt) {
    for (int aIndex = 0; aIndex < nroActors; aIndex++) {
      log(Info, "## ACTOR ", actors[aIndex]->getName());
      bool match = clock->matches(aIndex);
      actors[aIndex]->cycle(match);
    }
  } else if (setPressed) {
    nextInfoState();
  }
  updateInfo(lcdUp, lcdDown);
  stdOutWriteString(lcdUp, lcdDown);
}

void Bot::toConfigActorsMode(BotModeData *data, bool modePressed, bool setPressed, bool timerInterrupt) {
  char lcdUp[LCD_LENGTH + 1];
  char lcdDown[LCD_LENGTH + 1];
  if (modePressed) {
    nextActorConfigState();
    if (!canChangeMode) { // not yet done with actors configuration
      int nroActorConfigs = actors[actorIndex]->getNroConfigs();
      sprintf(lcdUp, "%s %s", data->lcdMessage, actors[actorIndex]->getName());
      if (actorStateIndex < nroActorConfigs) { // standard actor configs
        actors[actorIndex]->setConfig(actorStateIndex, lcdDown, DO_NOT_CHANGE);
      } else { // non standard actor config: frequency
        sprintf(lcdDown, "%s %s", MSG_BOT_FREQUENCY_SET, clock->getFrequencyDescription(actorIndex));
      }
    } else { // done with actors configuration
      sprintf(lcdUp, "%s", data->lcdMessage);
      sprintf(lcdDown, MSG_BOT_DONE_CONFIGURING_ACTORS);
    }
  } else if (setPressed && !canChangeMode) { // set pressed and not done with actors
    sprintf(lcdUp, "%s %s", data->lcdMessage, actors[actorIndex]->getName());
    int nroActorConfigs = actors[actorIndex]->getNroConfigs();
    if (actorStateIndex < nroActorConfigs) { // standard actor configs
      actors[actorIndex]->setConfig(actorStateIndex, lcdDown, DO_CHANGE);
    } else {
      clock->setNextFrequency(actorIndex);
      sprintf(lcdDown, "%s %s", MSG_BOT_FREQUENCY_SET, clock->getFrequencyDescription(actorIndex));
    }
  }
  if (modePressed || setPressed) {
    stdOutWriteString(lcdUp, lcdDown);
  }
}

void Bot::toConfigHourMode(BotModeData *data, bool modePressed, bool setPressed, bool timerInterrupt) {
  if (setPressed) {
    clock->increaseHour();
  }
  char timeBuffer[LCD_LENGTH + 1];
  char lcdDown[LCD_LENGTH + 1];
  clock->populateWithTime(timeBuffer);
  sprintf(lcdDown, "%s %s", MSG_BOT_HOUR_SET, timeBuffer);
  stdOutWriteString(data->lcdMessage, lcdDown);
}

void Bot::toConfigMinuteMode(BotModeData *data, bool modePressed, bool setPressed, bool timerInterrupt) {
  if (setPressed) {
    clock->increaseMinute();
  }
  char timeBuffer[LCD_LENGTH + 1];
  char lcdDown[LCD_LENGTH + 1];
  clock->populateWithTime(timeBuffer);
  sprintf(lcdDown, "%s %s", MSG_BOT_MINUTE_SET, timeBuffer);
  stdOutWriteString(data->lcdMessage, lcdDown);
}

void Bot::toConfigFactorMode(BotModeData *data, bool modePressed, bool setPressed, bool timerInterrupt) {
  if (setPressed) {
    clock->increaseFactor();
  }
  char lcdUp[LCD_LENGTH + 1];
  char lcdDown[LCD_LENGTH + 1];
  float factor = clock->getFactor();
  sprintf(lcdUp, "%s %s%d", data->lcdMessage, MSG_BOT_FACTOR_SET, (int)(factor * 10000));
  clock->populateWithTime(lcdDown);
  stdOutWriteString(lcdUp, lcdDown);
}

void Bot::nextActorConfigState() {
  if (canChangeMode) { // just arrived to the config actors state
    canChangeMode = false;
    actorIndex = 0;
    actorStateIndex = 0;
  } else { // were here from previous cycle
    int nroActorConfigs = actors[actorIndex]->getNroConfigs();
    actorStateIndex++;
    if (actorStateIndex == nroActorConfigs + 1) { // no more actor configuration states
      actorIndex++;
      actorStateIndex = 0;
      if (actorIndex == nroActors) { // done with actors configuration
        canChangeMode = true;
        // initialize for info states: clock display
        actorIndex = nroActors;
        actorStateIndex = 0;
      }
    }
  }
}
void Bot::updateInfo(char *lcdUp, char *lcdDown) {
  if (actorIndex < nroActors) { // infos for actors
    int nroActorInfoStates = actors[actorIndex]->getNroInfos();
    sprintf(lcdUp, "%s %s", MSG_BOT_RUN_STATE, actors[actorIndex]->getName()); // LCDUP: RUN ACTOR0
    if (actorStateIndex < nroActorInfoStates) {                                // actor infos
      actors[actorIndex]->getInfo(actorStateIndex, lcdDown);
    } else if (actorStateIndex == nroActorInfoStates) { // frequency infos
      sprintf(lcdDown, "%s %s", MSG_BOT_FREQUENCY_INFO, clock->getFrequencyDescription(actorIndex));
    }
  } else if (actorIndex == nroActors) { // general infos
    switch (actorStateIndex) {
      case ClockInfo:
        sprintf(lcdUp, "%s %s", MSG_BOT_RUN_STATE, MSG_BOT_CLOCK);
        clock->populateWithTime(lcdDown);
        log(Debug, "TIME:", lcdDown);
        break;
      default:
        break;
    }
  }
}

void Bot::nextInfoState() {
  if (actorIndex < nroActors) { // infos for actors
    int nroActorInfoStates = actors[actorIndex]->getNroInfos();
    actorStateIndex++;
    if (actorStateIndex == nroActorInfoStates + 1) {
      actorIndex++;
      actorStateIndex = 0;
    }
  } else if (actorIndex == nroActors) { // general infos
    actorStateIndex++;
    if (actorStateIndex == DelimiterBotInfo) {
      actorIndex++;
      actorStateIndex = 0;
    }
  }
  if (actorIndex == nroActors + 1) { // reset
    // initialize for config states
    actorIndex = 0;
    actorStateIndex = 0;
  }
}

Clock *Bot::getClock() {
  return clock;
}
