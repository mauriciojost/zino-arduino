#include <Debug.h>
#include <Misc.h>

#define INTERNAL_PERIOD_TO_SECONDS_FACTOR 8.0f

#define MIN_WATER_PERIOD_HOURS 1
#define DEFAULT_WATER_PERIOD_HOURS 1
#define MAX_WATER_PERIOD_HOURS 24 * 15
#define WATERING_TIME_SECONDS INTERNAL_PERIOD_TO_SECONDS_FACTOR * 2
#define PARKING_TIME_SECONDS INTERNAL_PERIOD_TO_SECONDS_FACTOR * 2

#define MIN_WATER_AMOUNT_PER_SHOT 1
#define DEFAULT_WATER_AMOUNT_PER_SHOT 10
#define MAX_WATER_AMOUNT_PER_SHOT 100


enum BotState { RunState, WelcomeState, ConfigPeriodState, ConfigAmountState }; // defines the sequence of modes too

class Bot {

public:
  BotState state;
  uint32_t waterPeriodHours; // expressed in hours
  uint32_t waterAmountPerShot; // expressed in <to be defined>
  uint32_t waterCurrentAmount; // expressed in <to be defined>
  uint32_t waterTimerCounter; // expressed in timer periods (~8 seconds each)
  bool isServoDriven;
  uint32_t servoPosition;
  uint32_t maxServoPosition;
  void (*stdOutWriteString)(const char *, const char *);

  Bot(void (*wrSt)(const char *, const char *)) {
    this->state = WelcomeState;
    this->waterPeriodHours = DEFAULT_WATER_PERIOD_HOURS;
    this->waterAmountPerShot = DEFAULT_WATER_AMOUNT_PER_SHOT;
    this->waterTimerCounter = 0;
    this->stdOutWriteString = wrSt;
    this->isServoDriven = false;
    this->servoPosition = 0;
    this->maxServoPosition = 0;
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
    uint32_t secondsFromBeginning = this->waterTimerCounter * INTERNAL_PERIOD_TO_SECONDS_FACTOR;
    toDayHourMinutesString(secondsFromBeginning, waterTimerMsg);
    char str[16];
    sprintf(str, "%s %d", waterTimerMsg, this->maxServoPosition);
    this->stdOutWriteString("RUN", str);

  }

  void toConfigPeriodState() {
    debug("TO CONFIG PERIOD...");
    setState(ConfigPeriodState);
    char str[16];
    sprintf(str, "%02d hours", this->waterPeriodHours);
    this->stdOutWriteString("WATER EVERY...?", str);
  }

  void toConfigAmountState() {
    debug("TO CONFIG AMOUNT...");
    setState(ConfigAmountState);
    char str[16];
    sprintf(str, "%02d degrees", this->waterAmountPerShot);
    this->stdOutWriteString("HOW MUCH WATER?", str);
  }

  void waterTimeMaybe() {
    this->waterTimerCounter += 1;
    uint32_t secondsFromBeginning = this->waterTimerCounter * INTERNAL_PERIOD_TO_SECONDS_FACTOR;
    uint32_t secondsWhenToWater = waterPeriodHours * 3600;
    if (secondsFromBeginning > secondsWhenToWater) {
      debug("  SERVO: WATERING");
      this->waterTimerCounter = 0; // reset water timer counter
      this->maxServoPosition = constrainValue(this->maxServoPosition + this->waterAmountPerShot, 0, 180);
      this->servoPosition = this->maxServoPosition;
      this->isServoDriven = true; // force watering system behaviour (follow aperture)
    } else if (secondsFromBeginning <= WATERING_TIME_SECONDS) {
      debug("  SERVO: STILL WATERING");
      this->isServoDriven = true; // force watering system behaviour (follow aperture)
    } else if (secondsFromBeginning <= WATERING_TIME_SECONDS + PARKING_TIME_SECONDS) {
      debug("  SERVO: PARKING");
      this->servoPosition = 0;
      this->isServoDriven = true; // force watering system behaviour (parking)
    } else {
      debug("  SERVO: RELEASED");
      this->isServoDriven = false; // release watering system
    }
  }

  void increaseWaterPeriod() {
    this->waterPeriodHours =
        rollValue(this->waterPeriodHours + 1, MIN_WATER_PERIOD_HOURS,
                       MAX_WATER_PERIOD_HOURS);
  }

  void increaseWaterAmount() {
    this->waterAmountPerShot =
        rollValue(this->waterAmountPerShot + 1, MIN_WATER_AMOUNT_PER_SHOT,
                       MAX_WATER_AMOUNT_PER_SHOT);
  }

  void setState(BotState newState) { this->state = newState; }

};
