#include <Debug.h>
#include <Misc.h>

#define INTERNAL_PERIOD_TO_SECONDS_FACTOR 8.0f
#define BUTTON_DEBOUNCING_DELAY_MS 100

#define FULL_FRACTION 1.0f
#define EMPTY_FRACTION 0.0f

#define MIN_WATER_PERIOD_HOURS 6
#define DEFAULT_WATER_PERIOD_HOURS 6
#define MAX_WATER_PERIOD_HOURS 24 * 15
#define INCR_WATER_PERIOD_HOURS 6

#define EXTRA_WATERING_CYCLES 1
#define EXTRA_PARKING_CYCLES 1

#define MIN_WATER_AMOUNT_PER_SHOT 0.05f
#define DEFAULT_WATER_AMOUNT_PER_SHOT 0.10f
#define MAX_WATER_AMOUNT_PER_SHOT 1.00f
#define INCR_WATER_AMOUNT_PER_SHOT 0.05f

enum BotState {
  RunState = 0,
  WelcomeState = 1,
  ConfigPeriodState = 2,
  ConfigAmountState = 3,
  DelimiterAmountOfStates = 4
};

struct BotStateData { BotState currentState; const char* lcdMessage; BotState nextState; };

BotStateData statesData[DelimiterAmountOfStates] = {
  { RunState, "RUNNING...", ConfigPeriodState},
  { WelcomeState, "WELCOME!", ConfigPeriodState},
  { ConfigPeriodState, "FREQUENCY?", ConfigAmountState},
  { ConfigAmountState, "WATER/SHOT?", RunState}
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
      default: break;
    }
  }

  void toWelcomeState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt);
  void toRunState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt);
  void toConfigPeriodState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt);
  void toConfigAmountState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt);

  void waterTimeMaybe();
  void increaseWaterPeriod();
  void increaseWaterAmount();

public:
  BotState state;
  uint32_t waterPeriodHours; // expressed in hours
  float waterAmountPerShot; // expressed in fraction of capacity
  float waterCurrentAmount; // expressed in fraction of capacity remaining
  uint32_t waterTimerCounter; // expressed in timer periods (~8 seconds each)
  bool isServoDriven;
  uint32_t servoPosition; // expressed in degrees
  uint32_t maxServoPosition; // expressed in degrees
  void (*stdOutWriteString)(const char *, const char *);

  Bot(void (*wrSt)(const char *, const char *));
  void run(bool modePressed, bool setPressed, bool timerInterrupt);


};

void Bot::run(bool modePressed, bool setPressed, bool timerInterrupt) {
  if (modePressed) {
    BotState nextState = statesData[this->state].nextState;
    this->state = nextState;
    debug("->NS"); debug(nextState);
    doTransition(nextState, modePressed, setPressed, timerInterrupt); // transition
  } else {
    debug("->SS"); debug(this->state);
    doTransition(this->state, modePressed, setPressed, timerInterrupt); // keep in the same state
  }
  if (modePressed || setPressed) {
    delay(BUTTON_DEBOUNCING_DELAY_MS);
  }
}

Bot::Bot(void (*wrSt)(const char *, const char *)) {
  this->state = WelcomeState;
  this->waterPeriodHours = DEFAULT_WATER_PERIOD_HOURS;
  this->waterAmountPerShot = DEFAULT_WATER_AMOUNT_PER_SHOT;
  this->waterCurrentAmount = FULL_FRACTION;
  this->waterTimerCounter = 0;
  this->stdOutWriteString = wrSt;
  this->isServoDriven = false;
  this->servoPosition = 0;
  this->maxServoPosition = 0;
}


void Bot::toWelcomeState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  this->stdOutWriteString(data.lcdMessage, "");
}

void Bot::toRunState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  char dayHourMinutesBuffer[16];
  char dayHourMinutesRemainingBuffer[16];
  if (timerInterrupt) { waterTimeMaybe(); }
  uint32_t secondsFromBeginning = this->waterTimerCounter * INTERNAL_PERIOD_TO_SECONDS_FACTOR;
  toDayHourMinutesString(secondsFromBeginning, dayHourMinutesBuffer);
  sprintf(dayHourMinutesRemainingBuffer, "%s %d%%", dayHourMinutesBuffer, (int)(fractionRemainingWater(this->maxServoPosition) * 100));
  this->stdOutWriteString(data.lcdMessage, dayHourMinutesRemainingBuffer);

}

void Bot::toConfigPeriodState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  char hoursBuffer[16];
  if (setPressed) { increaseWaterPeriod(); }
  sprintf(hoursBuffer, "%02d hours", this->waterPeriodHours);
  this->stdOutWriteString(data.lcdMessage, hoursBuffer);
}

void Bot::toConfigAmountState(BotStateData data, bool modePressed, bool setPressed, bool timerInterrupt) {
  char waterAmountBuffer[16];
  if (setPressed) { increaseWaterAmount(); }
  sprintf(waterAmountBuffer, "%d%%", (int)(this->waterAmountPerShot * 100));
  this->stdOutWriteString(data.lcdMessage, waterAmountBuffer);
}

void Bot::waterTimeMaybe() {
  this->waterTimerCounter += 1;
  uint32_t secondsFromBeginning = this->waterTimerCounter * INTERNAL_PERIOD_TO_SECONDS_FACTOR;
  uint32_t secondsWhenToWater = waterPeriodHours * 3600;
  if (secondsFromBeginning >= secondsWhenToWater) {
    debug("  SVO: WAT");
    this->waterTimerCounter = 0; // reset water timer counter
    this->maxServoPosition = calculateNewServoPosition(this->maxServoPosition, this->waterAmountPerShot);
    this->servoPosition = this->maxServoPosition;
    this->isServoDriven = true; // force watering system behaviour (follow aperture)
  } else if (this->waterTimerCounter <= EXTRA_WATERING_CYCLES) {
    debug("  SVO: WAT(2)");
    this->isServoDriven = true; // force watering system behaviour (follow aperture)
  } else if (this->waterTimerCounter <= EXTRA_WATERING_CYCLES + EXTRA_PARKING_CYCLES) {
    debug("  SVO: PKG");
    this->servoPosition = 0;
    this->isServoDriven = true; // force watering system behaviour (parking)
  } else {
    debug("  SVO: DEA");
    this->isServoDriven = false; // release watering system
  }
}

void Bot::increaseWaterPeriod() {
  this->waterPeriodHours =
      rollValue(this->waterPeriodHours + INCR_WATER_PERIOD_HOURS, MIN_WATER_PERIOD_HOURS,
                     MAX_WATER_PERIOD_HOURS);
}

void Bot::increaseWaterAmount() {
  this->waterAmountPerShot =
      rollValue(this->waterAmountPerShot + INCR_WATER_AMOUNT_PER_SHOT, MIN_WATER_AMOUNT_PER_SHOT,
                     MAX_WATER_AMOUNT_PER_SHOT);
}
