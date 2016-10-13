#include <Bot.h>

BotStateData statesData[DelimiterAmountOfBotStates] = {
    {RunState, "RUNNING...", ConfigPeriodState},
    {WelcomeState, "WELCOME!", ConfigPeriodState},
    {ConfigPeriodState, "FREQUENCY?", ConfigAmountState},
    {ConfigAmountState, "WATER/SHOT?", ConfigHourState},
    {ConfigHourState, "HOUR?", ConfigMinuteState},
    {ConfigMinuteState, "MINUTE?", ConfigFilledState},
    {ConfigFilledState, "JUST FILLED?", ServoTestState},
    {ServoTestState, "SERVO TEST", RunState}};

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
    BotState nextState = statesData[this->state].nextState;
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

// PRIVATE

void Bot::doTransition(BotState toState, bool modePressed, bool setPressed,
                       bool timerInterrupt) {
  BotStateData data = statesData[toState];
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
  case ConfigHourState:
    toConfigHourState(data, modePressed, setPressed, timerInterrupt);
    break;
  case ConfigMinuteState:
    toConfigMinuteState(data, modePressed, setPressed, timerInterrupt);
    break;
  case ConfigFilledState:
    toConfigFilledState(data, modePressed, setPressed, timerInterrupt);
    break;
  case ServoTestState:
    toServoTestState(data, modePressed, setPressed, timerInterrupt);
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
  char dayHourMinutesRemainingBuffer[16];
  if (timerInterrupt) {
    this->barrel.cycle(this->clock.matches());
  }
  sprintf(dayHourMinutesRemainingBuffer, "%d d %02d:%02d:%02d %d%%",
          (int)this->clock.getDays(), (int)this->clock.getHours(),
          (int)this->clock.getMinutes(), (int)this->clock.getSeconds(),
          (int)(this->barrel.remainingWater() * 100));
  this->stdOutWriteString(data.lcdMessage, dayHourMinutesRemainingBuffer);
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
  char waterAmountBuffer[16];
  if (setPressed) {
    increaseWaterAmount();
  }
  sprintf(waterAmountBuffer, "%d%%", (int)(this->barrel.waterAmountPerShot * 100));
  this->stdOutWriteString(data.lcdMessage, waterAmountBuffer);
}

void Bot::toConfigHourState(BotStateData data, bool modePressed,
                            bool setPressed, bool timerInterrupt) {
  if (setPressed) {
    this->clock.increaseHour();
  }
  char buffer[16];
  this->clock.getTimeString(buffer);
  this->stdOutWriteString(data.lcdMessage, buffer);
}

void Bot::toConfigMinuteState(BotStateData data, bool modePressed,
                              bool setPressed, bool timerInterrupt) {
  if (setPressed) {
    this->clock.increaseMinute();
  }
  char buffer[16];
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

void Bot::toServoTestState(BotStateData data, bool modePressed,
                              bool setPressed, bool timerInterrupt) {
  // TODO fix
  //char buffer[16];
  //if (setPressed) {
      //this->servoPosition = rollValue(this->servoPosition + 2, 0, 180);
  //}
  //sprintf(buffer, "SERVO: %d", (int)this->servoPosition);
  //this->stdOutWriteString(data.lcdMessage, buffer);
  //this->servoState = ServoDrivenState;
}

void Bot::increaseWaterAmount() {
  this->barrel.nextWaterAmountPerShot();
  log(Debug, "WATER:", (int)this->barrel.waterAmountPerShot);
}

