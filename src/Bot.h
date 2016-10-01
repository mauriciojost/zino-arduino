#define constrainValue(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
#define MIN_WATER_PERIOD_HOURS 1
#define MAX_WATER_PERIOD_HOURS 24 * 15
#define DEFAULT_WATER_PERIOD_HOURS 24 * 2

enum BotState{Welcome = 0, Config = 1, Run = 2};

const char* lcdMessageWelcome = "WELCOME         \0.RUN     CONFIG.";
const char* lcdMessageRun     = "RUN             \0.       WELCOME.";
const char* lcdMessageConfig  = "CONFIG          \0.WELCOME     ++.";

class Bot {

public:

  BotState state;
  uint32_t waterPeriodHours;
  const char* lcdMessage;
  void (*lcdWrite)(const char*) ;

  Bot(void (*l)(const char*)) {
    this->state = Welcome;
    this->waterPeriodHours = DEFAULT_WATER_PERIOD_HOURS;
    this->lcdWrite = l;
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
    case Welcome:
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
    this->lcdWrite(lcdMessageWelcome);
    setState(Welcome);
  }

  void toStateRun() {
    this->lcdWrite(lcdMessageRun);
    setState(Run);
  }

  void toStateConfig() {
    this->lcdWrite(lcdMessageConfig);
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

