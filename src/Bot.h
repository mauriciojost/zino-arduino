#include <Debug.h>
#include <Misc.h>
#include <Clock.h>

#define INTERNAL_PERIOD_TO_SECONDS_FACTOR 8.0f

#define FULL_FRACTION 1.0f
#define EMPTY_FRACTION 0.0f

#define MIN_WATER_AMOUNT_PER_SHOT 0.05f
#define DEFAULT_WATER_AMOUNT_PER_SHOT 0.10f
#define MAX_WATER_AMOUNT_PER_SHOT 1.00f
#define INCR_WATER_AMOUNT_PER_SHOT 0.05f

enum ServoState {
  ServoReleasedState = 0,
  ServoDrivenState = 1,
  ServoParkingState = 2,
  DelimiterAmountOfServoStates = 3
};

enum BotState {
  RunState = 0,
  WelcomeState = 1,
  ConfigPeriodState = 2,
  ConfigAmountState = 3,
  ConfigHourState = 4,
  ConfigMinuteState = 5,
  ConfigFilledState = 6,
  DelimiterAmountOfBotStates = 7
};

struct BotStateData { BotState currentState; const char* lcdMessage; BotState nextState; };

BotStateData statesData[DelimiterAmountOfBotStates] = {
  { RunState, "RUNNING...", ConfigPeriodState},
  { WelcomeState, "WELCOME!", ConfigPeriodState},
  { ConfigPeriodState, "FREQUENCY?", ConfigAmountState},
  { ConfigAmountState, "WATER/SHOT?", ConfigHourState},
  { ConfigHourState, "HOUR?", ConfigMinuteState},
  { ConfigMinuteState, "MINUTE?", ConfigFilledState},
  { ConfigFilledState, "JUST FILLED?", RunState}
};

class Bot {

private:

  void doTransition(BotState toState, bool modePressed, bool setPressed, bool timerInterrupt) {
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

  void toWelcomeState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt);
  void toRunState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt);
  void toConfigPeriodState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt);
  void toConfigAmountState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt);
  void toConfigHourState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt);
  void toConfigMinuteState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt);
  void toConfigFilledState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt);

  void waterTimeMaybe();
  void increaseWaterPeriod();
  void increaseWaterAmount();
  void increaseHour();
  void increaseMinute();
  void setToFilled();

public:
  Clock clock;
  BotState state;
  ServoState servoState; // state of the servo
  float waterAmountPerShot; // expressed in fraction of capacity
  float waterCurrentAmount; // expressed in fraction of capacity remaining
  int servoPosition; // expressed in degrees
  int maxServoPosition; // expressed in degrees
  void (*stdOutWriteString)(const char *, const char *);

  Bot(void (*wrSt)(const char *, const char *));
  void run(bool modePressed, bool setPressed, bool timerInterrupt);
  bool isServoDriven();


};

void Bot::run(bool modePressed, bool setPressed, bool timerInterrupt) {
  if (timerInterrupt) {
    this->clock.tick();
  }
  if (modePressed) {
    BotState nextState = statesData[this->state].nextState;
    this->state = nextState;
    debug("->NS:"); debug(nextState);
    doTransition(nextState, modePressed, setPressed, timerInterrupt); // transition
  } else {
    debug("->SS:"); debug(this->state);
    doTransition(this->state, modePressed, setPressed, timerInterrupt); // keep in the same state
  }
}

Bot::Bot(void (*wrSt)(const char *, const char *)) {
  this->state = WelcomeState;
  this->waterAmountPerShot = DEFAULT_WATER_AMOUNT_PER_SHOT;
  this->waterCurrentAmount = FULL_FRACTION;
  this->stdOutWriteString = wrSt;
  this->servoState = ServoDrivenState;
  this->servoPosition = 0;
  this->maxServoPosition = 0;
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
    debug("  SVO: ->PKG");
    this->servoPosition = 0;
    this->servoState = ServoParkingState;
  } else if (this->servoState == ServoParkingState) {
    debug("  SVO: ->RLS");
    this->servoPosition = 0;
    this->servoState = ServoReleasedState;
  } else if (this->clock.isTimeToWater()) {
    debug("  SVO: ->WAT");
    this->maxServoPosition = calculateNewServoPosition(this->maxServoPosition, this->waterAmountPerShot);
    this->servoPosition = this->maxServoPosition;
    this->servoState = ServoDrivenState;
  } else {
    debug("  SVO: zzz");
    this->servoState = ServoReleasedState;
  }
  debug("  SVO: state ", this->servoState);
  debug("  SVO: position ", this->servoPosition);
}

bool Bot::isServoDriven() {
  return (this->servoState == ServoDrivenState) || (this->servoState == ServoParkingState);
}

void Bot::increaseWaterPeriod() {
  this->clock.setNextFrequency();
}

void Bot::increaseWaterAmount() {
  this->waterAmountPerShot =
      rollValue(this->waterAmountPerShot + INCR_WATER_AMOUNT_PER_SHOT, MIN_WATER_AMOUNT_PER_SHOT,
                     MAX_WATER_AMOUNT_PER_SHOT);
}

void Bot::increaseHour() {
  unsigned int h = this->clock.getHours();
  unsigned int m = this->clock.getMinutes();
  this->clock.set(0, rollValue(h + 1, 0, 24), m, 0);
}

void Bot::increaseMinute() {
  unsigned int h = this->clock.getHours();
  unsigned int m = this->clock.getMinutes();
  this->clock.set(0, h, rollValue(m + 1, 0, 60), 0);
}
void Bot::setToFilled() {
  this->maxServoPosition = 0;
}

