#include <Bot.h>
#include <ui/Messages.h>

#define CLASS "Bot"

#define DO_CHANGE true
#define DO_NOT_CHANGE false

Bot::Bot(Clock *clk, Actor **a, int nActors, Configurable **c, int nConfigurables) {
  nroActors = nActors;
  actors = a;
  nroConfigurables = nConfigurables;
  configurables = c;
  clock = clk;
  mode = WelcomeMode;
  canChangeMode = true;
  configurableIndex = 0;
  configurableStateIndex = 0;
}

void Bot::setStdoutFunction(void (*wrSt)(const char *, const char *)) {
  stdOutWriteStringFunction = wrSt;
}

void Bot::cycle(bool modePressed, bool setPressed, bool timerInterrupt) {
  if (timerInterrupt) {
    clock->cycle();
  }
  BotMode nextMode = mode; // no changes by default
  if (modePressed && canChangeMode) {
    nextMode = modesData[mode].nextMode;
    mode = nextMode;
    log(CLASS, Info, "->(NEXT) ST: ", modesData[mode].lcdMessage);
  } else {
    log(CLASS, Info, "->(SAME) ST: ", modesData[mode].lcdMessage);
  }
  (this->*modesData[nextMode].currentModeFunction)(&modesData[nextMode], modePressed, setPressed, timerInterrupt);
}

void Bot::setMode(BotMode s) {
  mode = s;
}

int Bot::getMode() {
  return mode;
}

int Bot::getConfigurableIndex() {
  return configurableIndex;
}

int Bot::getConfigurableStateIndex() {
  return configurableStateIndex;
}

void Bot::toWelcomeMode(BotModeData *data, bool modePressed, bool setPressed, bool timerInterrupt) {
  stdOutWriteString(data->lcdMessage, "");
}

void Bot::toHelpMode(BotModeData *data, bool modePressed, bool setPressed, bool timerInterrupt) {
  stdOutWriteString(MSG_BOT_STATE_HELP_UP, MSG_BOT_STATE_HELP_DOWN);
}

void Bot::toRunMode(BotModeData *data, bool modePressed, bool setPressed, bool timerInterrupt) {
  char lcdUp[LCD_LENGTH + 1];
  char lcdDown[LCD_LENGTH + 1];
  if (timerInterrupt) {
    for (int aIndex = 0; aIndex < nroActors; aIndex++) {
      log(CLASS, Info, "## ACTOR ", actors[aIndex]->getName());
      bool match = clock->matches(aIndex);
      actors[aIndex]->cycle(match);
    }
  } else if (setPressed) {
    nextInfoState();
  }
  updateInfo(lcdUp, lcdDown);
  stdOutWriteString(lcdUp, lcdDown);
}

void Bot::toConfigConfigurablesMode(BotModeData *data, bool modePressed, bool setPressed, bool timerInterrupt) {
  char lcdUp[LCD_LENGTH + 1];
  char lcdDown[LCD_LENGTH + 1];
  if (modePressed) {
    nextConfigurableConfigState();
    if (!canChangeMode) { // not yet done with configurables configuration
      int nroConfigurableConfigs = configurables[configurableIndex]->getNroConfigs();
      sprintf(lcdUp, "%s%s...", data->lcdMessage, configurables[configurableIndex]->getName());
      configurables[configurableIndex]->setConfig(configurableStateIndex, lcdDown, DO_NOT_CHANGE);
    } else { // done with actors configuration
      sprintf(lcdUp, "%s", data->lcdMessage);
      sprintf(lcdDown, MSG_BOT_DONE_CONFIGURING_ACTORS);
    }
  } else if (setPressed && !canChangeMode) { // set pressed and not done with configurables
    sprintf(lcdUp, "%s%s...", data->lcdMessage, configurables[configurableIndex]->getName());
    int nroConfigurableConfigs = configurables[configurableIndex]->getNroConfigs();
    configurables[configurableIndex]->setConfig(configurableStateIndex, lcdDown, DO_CHANGE);
  }
  if (modePressed || setPressed) {
    stdOutWriteString(lcdUp, lcdDown);
  }
}

void Bot::nextConfigurableConfigState() {
  if (canChangeMode) { // just arrived to the config actors state
    canChangeMode = false;
    configurableIndex = 0;
    configurableStateIndex = 0;
  } else { // were here from previous cycle
    int numConfigs = configurables[configurableIndex]->getNroConfigs();
    configurableStateIndex++;
    if (configurableStateIndex == numConfigs) { // no more configuration states for this configurable
      configurableIndex++;
      configurableStateIndex = 0;
      if (configurableIndex == nroConfigurables) { // done with actors configuration
        canChangeMode = true;
        configurableIndex = 0;
        configurableStateIndex = 0;
      }
    }
  }
}

void Bot::toConfigActorFrequenciesMode(BotModeData *data, bool modePressed, bool setPressed, bool timerInterrupt) {
  char lcdUp[LCD_LENGTH + 1];
  char lcdDown[LCD_LENGTH + 1];
  if (modePressed) {
    nextActor();
    if (!canChangeMode) { // not yet done with actors frequencies configuration
      sprintf(lcdUp, "%s%s...", data->lcdMessage, actors[configurableIndex]->getName());
      sprintf(lcdDown, "%s %s", MSG_BOT_FREQUENCY_SET, clock->getFrequencyDescription(configurableIndex));
    } else { // done with actors frequency configuration
      sprintf(lcdUp, "%s", data->lcdMessage);
      sprintf(lcdDown, MSG_BOT_DONE_CONFIGURING_ACTORS);
    }
  } else if (setPressed && !canChangeMode) { // set pressed and not done with actors frequency configuration
    sprintf(lcdUp, "%s%s...", data->lcdMessage, configurables[configurableIndex]->getName());
    clock->setNextFrequency(configurableIndex);
    sprintf(lcdDown, "%s %s", MSG_BOT_FREQUENCY_SET, clock->getFrequencyDescription(configurableIndex));
  }
  if (modePressed || setPressed) {
    stdOutWriteString(lcdUp, lcdDown);
  }
}

void Bot::nextActor() {
  if (canChangeMode) { // just arrived to the config actors state
    canChangeMode = false;
    configurableIndex = 0;
  } else { // were here from previous cycle
    configurableIndex++;
  }
  if (configurableIndex == nroActors) {
    canChangeMode = true;
    configurableIndex = 0;
  }
}

void Bot::updateInfo(char *lcdUp, char *lcdDown) {
  if (configurableIndex < nroConfigurables) { // infos for configurables
    int nroInfoStates = configurables[configurableIndex]->getNroInfos();
    sprintf(lcdUp, "%s %s", MSG_BOT_RUN_STATE, configurables[configurableIndex]->getName()); // LCDUP: RUN ACTOR0
    if (configurableStateIndex < nroInfoStates) {                                            // configurable infos
      configurables[configurableIndex]->getInfo(configurableStateIndex, lcdDown);
    }
  }
}

void Bot::nextInfoState() {
  if (configurableIndex < nroConfigurables) { // infos for configurables
    int nroInfoStates = configurables[configurableIndex]->getNroInfos();
    configurableStateIndex++;
    if (configurableStateIndex == nroInfoStates) { // number of info states for this configurable
      configurableIndex++;
      configurableStateIndex = 0;
    }
  }
  if (configurableIndex == nroConfigurables) { // reset indexes
    configurableIndex = 0;
    configurableStateIndex = 0;
  }
}

Clock *Bot::getClock() {
  return clock;
}

void Bot::stdOutWriteString(const char *up, const char *down) {
  if (stdOutWriteStringFunction != NULL) {
    stdOutWriteStringFunction(up, down);
  }
}
