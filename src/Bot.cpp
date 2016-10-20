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
  BotState nextState = this->state; // no changes by default
  if (modePressed) {
    log(Info, "->NEXT ST: ", (int)nextState);
    nextState = this->statesData[this->state].nextState;
    this->state = nextState;
  } else {
    log(Info, "->SAME ST: ", (int)this->state);
  }
  (this->*statesData[nextState].currentStateFunction)(
      this->statesData[nextState], modePressed, setPressed, timerInterrupt);
}

// PRIVATE

void Bot::toWelcomeState(BotStateData data, bool modePressed, bool setPressed,
                         bool timerInterrupt) {
  this->stdOutWriteString(data.lcdMessage, "");
}

void Bot::toRunState(BotStateData data, bool modePressed, bool setPressed,
                     bool timerInterrupt) {
  char buffer[16];
  if (timerInterrupt) {
    bool match = this->clock.matches();
    this->pump.cycle(match);
  }
  sprintf(buffer, "%dd %02d:%02d:%02d", (int)this->clock.getDays(),
          (int)this->clock.getHours(), (int)this->clock.getMinutes(),
          (int)this->clock.getSeconds());
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


