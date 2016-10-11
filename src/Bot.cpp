#include <Bot.h>

BotStateData statesData[DelimiterAmountOfBotStates] = {
  { RunState, "RUNNING...", ConfigPeriodState},
  { WelcomeState, "WELCOME!", ConfigPeriodState},
  { ConfigPeriodState, "FREQUENCY?", ConfigAmountState},
  { ConfigAmountState, "WATER/SHOT?", ConfigHourState},
  { ConfigHourState, "HOUR?", ConfigMinuteState},
  { ConfigMinuteState, "MINUTE?", ConfigFilledState},
  { ConfigFilledState, "JUST FILLED?", RunState}
};

// PUBLIC

Bot::Bot(void (*wrSt)(const char *, const char *)) {
  this->state = WelcomeState;
  this->waterAmountPerShot = DEFAULT_WATER_AMOUNT_PER_SHOT;
  this->waterCurrentAmount = FULL_FRACTION;
  this->stdOutWriteString = wrSt;
  this->servoState = ServoDrivenState;
  this->servoPosition = 0;
  this->maxServoPosition = 0;
}

void Bot::cycle(bool modePressed, bool setPressed, bool timerInterrupt) {
  if (timerInterrupt) {
    this->clock.cycle();
  }
  if (modePressed) {
    BotState nextState = statesData[this->state].nextState;
    this->state = nextState;
    log(Info, "->NS: ", (int)nextState);
    doTransition(nextState, modePressed, setPressed, timerInterrupt); // transition
  } else {
    log(Info, "->SS: ", (int)this->state);
    doTransition(this->state, modePressed, setPressed, timerInterrupt); // keep in the same state
  }
}

bool Bot::isServoDriven() {
  return (this->servoState == ServoDrivenState) || (this->servoState == ServoParkingState);
}

// PRIVATE

void Bot::doTransition(BotState toState, bool modePressed, bool setPressed, bool timerInterrupt) {
  BotStateData data = statesData[toState];
  switch (toState) {
    case RunState: toRunState(data, modePressed, setPressed, timerInterrupt); break;
    case WelcomeState: toWelcomeState(data, modePressed, setPressed, timerInterrupt); break;
    case ConfigPeriodState: toConfigPeriodState(data, modePressed, setPressed, timerInterrupt); break;
    case ConfigAmountState: toConfigAmountState(data, modePressed, setPressed, timerInterrupt); break;
    case ConfigHourState: toConfigHourState(data, modePressed, setPressed, timerInterrupt); break;
    case ConfigMinuteState: toConfigMinuteState(data, modePressed, setPressed, timerInterrupt); break;
    case ConfigFilledState: toConfigFilledState(data, modePressed, setPressed, timerInterrupt); break;
    default: break;
  }
}

void Bot::toWelcomeState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  this->stdOutWriteString(data.lcdMessage, "");
}

void Bot::toRunState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  char dayHourMinutesRemainingBuffer[16];
  if (timerInterrupt) { waterTimeMaybe(); }
  sprintf(dayHourMinutesRemainingBuffer, "%d d %02d:%02d:%02d %d%%",
    (int)this->clock.getDays(),
    (int)this->clock.getHours(),
    (int)this->clock.getMinutes(),
    (int)this->clock.getSeconds(),
    (int)(fractionRemainingWater(this->maxServoPosition) * 100));
  this->stdOutWriteString(data.lcdMessage, dayHourMinutesRemainingBuffer);

}

void Bot::toConfigPeriodState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  if (setPressed) { increaseWaterPeriod(); }
  this->stdOutWriteString(data.lcdMessage, this->clock.getFrequencyDescription());
}

void Bot::toConfigAmountState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  char waterAmountBuffer[16];
  if (setPressed) { increaseWaterAmount(); }
  sprintf(waterAmountBuffer, "%d%%", (int)(this->waterAmountPerShot * 100));
  this->stdOutWriteString(data.lcdMessage, waterAmountBuffer);
}

void Bot::toConfigHourState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  if (setPressed) { increaseHour(); }
  char buffer[16];
  sprintf(buffer, "%02d:%02d", (int)(this->clock.getHours()), (int)(this->clock.getMinutes()));
  this->stdOutWriteString(data.lcdMessage, buffer);
}

void Bot::toConfigMinuteState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  if (setPressed) { increaseMinute(); }
  char buffer[16];
  sprintf(buffer, "%02d:%02d", (int)(this->clock.getHours()), (int)(this->clock.getMinutes()));
  this->stdOutWriteString(data.lcdMessage, buffer);
}

void Bot::toConfigFilledState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  char buffer[16];
  if (setPressed) { setToFilled(); }
  sprintf(buffer, "FILLED: %d%%",
    (int)(fractionRemainingWater(this->maxServoPosition) * 100));
  this->stdOutWriteString(data.lcdMessage, buffer);
}

void Bot::waterTimeMaybe() {
  if (this->servoState == ServoDrivenState) {
    log(Debug, "  SVO: ->PKG");
    this->servoPosition = 0;
    this->servoState = ServoParkingState;
  } else if (this->servoState == ServoParkingState) {
    log(Debug, "  SVO: ->RLS");
    this->servoPosition = 0;
    this->servoState = ServoReleasedState;
  } else if (this->clock.matches()) {
    log(Debug, "  SVO: ->WAT");
    this->maxServoPosition = calculateNewServoPosition(this->maxServoPosition, this->waterAmountPerShot);
    this->servoPosition = this->maxServoPosition;
    this->servoState = ServoDrivenState;
  } else {
    log(Debug, "  SVO: zzz");
    this->servoState = ServoReleasedState;
  }
  log(Debug, "  SVO: state ", this->servoState);
  log(Debug, "  SVO: position ", this->servoPosition);
}

void Bot::increaseWaterPeriod() {
  this->clock.setNextFrequency();
}

void Bot::increaseWaterAmount() {
  this->waterAmountPerShot =
      rollValue(this->waterAmountPerShot + INCR_WATER_AMOUNT_PER_SHOT, MIN_WATER_AMOUNT_PER_SHOT,
                     MAX_WATER_AMOUNT_PER_SHOT);
  log(Debug, "WATER:", (int)this->waterAmountPerShot);
}

void Bot::increaseHour() {
  int h = this->clock.getHours();
  int m = this->clock.getMinutes();
  int nh = rollValue(h + 1, 0, 24);
  log(Debug, "H:", (int)h);
  log(Debug, "M:", (int)m);
  log(Debug, "NH:", (int)nh);
  this->clock.set(0, nh, m, 0);

}

void Bot::increaseMinute() {
  int h = this->clock.getHours();
  int m = this->clock.getMinutes();
  int nm = rollValue(m + 1, 0, 60);
  log(Debug, "H:", (int)h);
  log(Debug, "M:", (int)m);
  log(Debug, "NM:", (int)nm);
  this->clock.set(0, h, nm, 0);
}

void Bot::setToFilled() {
  this->maxServoPosition = 0;
}

