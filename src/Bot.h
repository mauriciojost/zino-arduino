#include <Debug.h>
#define constrainValue(amt, low, high)                                         \
  ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

#define MIN_WATER_PERIOD_MINUTES 1
#define DEFAULT_WATER_PERIOD_MINUTES 60 * 24 * 2
#define MAX_WATER_PERIOD_MINUTES 60 * 24 * 15

#define MIN_WATER_AMOUNT_PER_SHOT 1
#define DEFAULT_WATER_AMOUNT_PER_SHOT 10
#define MAX_WATER_AMOUNT_PER_SHOT 100

enum BotState { RunState, WelcomeState, ConfigPeriodState, ConfigAmountState }; // defines the sequence of modes too

class Bot {

public:
  BotState state;
  uint32_t waterPeriodMinutes;
  uint8_t waterAmountPerShot;
  uint32_t waterCurrentAmount;
  uint32_t waterTimerCounter;
  void (*stdOutWriteString)(const char *, const char *);

  Bot(void (*wrSt)(const char *, const char *)) {
    this->state = WelcomeState;
    this->waterPeriodMinutes = DEFAULT_WATER_PERIOD_MINUTES;
    this->waterAmountPerShot = DEFAULT_WATER_AMOUNT_PER_SHOT;
    this->waterTimerCounter = 0;
    this->stdOutWriteString = wrSt;
  }

  void run(bool modePressed, bool setPressed, bool timerInterrupt) {

    switch (this->state) {

    case RunState:
      if (modePressed) {
        toWelcomeState();
      } else if (timerInterrupt) {
        waterTimeMaybe();
        toRunState();
      } else {
        toRunState(); // Might need to remove it
      }
      break;
    case WelcomeState:
      if (modePressed) {
        toConfigPeriodState();
      } else {
        toWelcomeState(); // Might need to remove it
      }
      break;
    case ConfigPeriodState:
      if (modePressed) {
        toConfigAmountState();
      } else if (setPressed) {
        increaseWaterPeriod();
        toConfigPeriodState();
      } else {
        toConfigPeriodState(); // Might need to remove it
      }
      break;
    case ConfigAmountState:
      if (modePressed) {
        toRunState();
      } else if (setPressed) {
        increaseWaterAmount();
        toConfigAmountState();
      } else {
        toConfigAmountState(); // Might need to remove it
      }
      break;
    default:
      break;
    }
  }

private:
  void toWelcomeState() {
    debug("TO WELCOME...");
    setState(WelcomeState);
    this->stdOutWriteString("WELCOME", "");
  }

  void toRunState() {
    debug("TO RUN...");
    setState(RunState);

    char waterTimerMsg[16];
    sprintf(waterTimerMsg, "%d", this->waterTimerCounter);
    this->stdOutWriteString("RUN", waterTimerMsg);
  }

  void toConfigPeriodState() {
    debug("TO CONFIG PERIOD...");
    setState(ConfigPeriodState);
    this->stdOutWriteString("PERIOD", "");
  }

  void toConfigAmountState() {
    debug("TO CONFIG AMOUNT...");
    setState(ConfigAmountState);
    this->stdOutWriteString("AMOUNT", "");
  }

  void waterTimeMaybe() {
    this->waterTimerCounter += 8;
    if (waterTimerCounter > waterPeriodMinutes * 60) {
      this->waterTimerCounter = 0;
      // water here!
    }
  }

  void increaseWaterPeriod() {
    this->waterPeriodMinutes =
        constrainValue(this->waterPeriodMinutes + 1, MIN_WATER_PERIOD_MINUTES,
                       MAX_WATER_PERIOD_MINUTES);
  }

  void increaseWaterAmount() {
    this->waterAmountPerShot =
        constrainValue(this->waterAmountPerShot + 1, MIN_WATER_AMOUNT_PER_SHOT,
                       MAX_WATER_AMOUNT_PER_SHOT);
  }

  void setState(BotState newState) { this->state = newState; }

};
