#include <Bot.h>

// PUBLIC

Bot::Bot(void (*wrSt)(const char *, const char *)) {
  this->state = WelcomeState;
  this->stdOutWriteString = wrSt;
}

void Bot::cycle(bool modePressed, bool setPressed, bool timerInterrupt) {
  if (timerInterrupt) {
    this->clock.cycle();
  }
  if (modePressed) {
    BotState nextState = this->statesData[this->state].nextState;
    this->state = nextState;
    log(Info, "->NS: ", (int)nextState);
    doTransition(nextState, modePressed, setPressed,
                 timerInterrupt); // transition
  } else {
    log(Info, "->SS: ", (int)this->state);
    doTransition(this->state, modePressed, setPressed,
                 timerInterrupt); // keep in the same state
  }
}

bool Bot::isServoDriven() {
  return this->barrel.isServoDriven() && this->state == RunState;
}

bool Bot::isPumpDriven() {
  return this->pump.isPumpDriven() && this->state == RunState;
}

// PRIVATE

void Bot::doTransition(BotState toState, bool modePressed, bool setPressed,
                       bool timerInterrupt) {
  BotStateData data = this->statesData[toState];
  switch (toState) {
  case RunState:
    toRunState(data, modePressed, setPressed, timerInterrupt);
    break;
  case WelcomeState:
    toWelcomeState(data, modePressed, setPressed, timerInterrupt);
    break;
  case ConfigPeriodState:
    toConfigPeriodState(data, modePressed, setPressed, timerInterrupt);
    break;
  case ConfigAmountState:
    toConfigAmountState(data, modePressed, setPressed, timerInterrupt);
    break;
  case ConfigAmountPumpState:
    toConfigAmountPumpState(data, modePressed, setPressed, timerInterrupt);
    break;
  case ConfigHourState:
    toConfigHourState(data, modePressed, setPressed, timerInterrupt);
    break;
  case ConfigMinuteState:
    toConfigMinuteState(data, modePressed, setPressed, timerInterrupt);
    break;
  case ConfigFilledState:
    toConfigFilledState(data, modePressed, setPressed, timerInterrupt);
    break;
  default:
    break;
  }
}

void Bot::toWelcomeState(BotStateData data, bool modePressed, bool setPressed,
                         bool timerInterrupt) {
  this->stdOutWriteString(data.lcdMessage, "");
}

void Bot::toRunState(BotStateData data, bool modePressed, bool setPressed,
                     bool timerInterrupt) {
  char buffer[16];
  if (timerInterrupt) {
    bool match = this->clock.matches();
    this->barrel.cycle(match);
    this->pump.cycle(match);
  }
  sprintf(buffer, "%dd %02d:%02d:%02d %d%%",
          (int)this->clock.getDays(), (int)this->clock.getHours(),
          (int)this->clock.getMinutes(), (int)this->clock.getSeconds(),
          (int)(this->barrel.remainingWater() * 100));
  this->stdOutWriteString(data.lcdMessage, buffer);
}

void Bot::toConfigPeriodState(BotStateData data, bool modePressed,
                              bool setPressed, bool timerInterrupt) {
  if (setPressed) {
    this->clock.setNextFrequency();
  }
  this->stdOutWriteString(data.lcdMessage,
                          this->clock.getFrequencyDescription());
}

void Bot::toConfigAmountState(BotStateData data, bool modePressed,
                              bool setPressed, bool timerInterrupt) {
  char buffer[16];
  if (setPressed) {
    this->barrel.nextWaterAmountPerShot();
    log(Debug, "WATER:", (int)this->barrel.waterAmountPerShot);
  }
  sprintf(buffer, "%d%%", (int)(this->barrel.waterAmountPerShot * 100));
  this->stdOutWriteString(data.lcdMessage, buffer);
}

void Bot::toConfigAmountPumpState(BotStateData data, bool modePressed,
                              bool setPressed, bool timerInterrupt) {
  char buffer[16];
  if (setPressed) {
    this->pump.nextWaterAmountPerShot();
    log(Debug, "WATER:", (int)this->pump.waterAmountPerShot);
  }
  sprintf(buffer, "%d sec", (int)(this->pump.waterAmountPerShot));
  this->stdOutWriteString(data.lcdMessage, buffer);
}

void Bot::toConfigHourState(BotStateData data, bool modePressed,
                            bool setPressed, bool timerInterrupt) {
  char buffer[16];
  if (setPressed) {
    this->clock.increaseHour();
  }
  this->clock.getTimeString(buffer);
  this->stdOutWriteString(data.lcdMessage, buffer);
}

void Bot::toConfigMinuteState(BotStateData data, bool modePressed,
                              bool setPressed, bool timerInterrupt) {
  char buffer[16];
  if (setPressed) {
    this->clock.increaseMinute();
  }
  this->clock.getTimeString(buffer);
  this->stdOutWriteString(data.lcdMessage, buffer);
}

void Bot::toConfigFilledState(BotStateData data, bool modePressed,
                              bool setPressed, bool timerInterrupt) {
  char buffer[16];
  if (setPressed) {
    this->barrel.setToFilled();
  }
  sprintf(buffer, "FILLED: %d%%",
          (int)(this->barrel.remainingWater() * 100));
  this->stdOutWriteString(data.lcdMessage, buffer);
}

