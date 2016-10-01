
#include <Display.h>

#define constrainValue(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define MIN_WATER_PERIOD_HOURS 1
#define MAX_WATER_PERIOD_HOURS 24 * 15
#define DEFAULT_WATER_PERIOD_HOURS 24 * 2

enum BotState{Display, Config, Run};

const char* lcdMessageDisplay = "DISPLAY (RUN | CONFIG)";
const char* lcdMessageRun     = "RUN (X | DISPLAY)";
const char* lcdMessageConfig  = "CONFIG (DISPLAY | ++)";

class Bot {

public:

  BotState state;
  uint32_t waterPeriodHours;
  const char* lcdMessage;

  Bot() {
    this->state = Display;
    this->lcdMessage = lcdMessageDisplay;
    this->waterPeriodHours = DEFAULT_WATER_PERIOD_HOURS;
  }

  // Interruptions in case of button pressed or timer
  void run(bool b0, bool b1, bool t0) {

    switch (this->state) {

    case Run:
      if (b1) {
        toStateDisplay();
      } else if (t0) {
        goWater();
        toStateRun();
      }
      break;
    case Display:
      if (b0) {
        toStateRun();
      } else if (b1) {
        toStateConfig();
      }
      break;
    case Config:
      if (b0) {
        toStateDisplay();
      } else if (b1) {
        increaseWaterPeriod();
        toStateConfig();
      }
      break;
    default:
      break;
    }
  }


private:

  void toStateDisplay() {
    display(lcdMessageDisplay);
    setState(Display);
  }

  void toStateRun() {
    display(lcdMessageRun);
    setState(Run);
  }

  void toStateConfig() {
    display(lcdMessageConfig);
    setState(Config);
  }

  void goWater() {
    // do the watering
  }

  void increaseWaterPeriod() {
    this->waterPeriodHours =
        constrainValue(this->waterPeriodHours + 1, MIN_WATER_PERIOD_HOURS,
                  MAX_WATER_PERIOD_HOURS);
  }

  void setState(BotState newState) { this->state = newState; }

};

