#define constrainValue(amt, low, high)                                         \
  ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))
#define MIN_WATER_PERIOD_HOURS 1
#define MAX_WATER_PERIOD_HOURS 24 * 15
#define DEFAULT_WATER_PERIOD_HOURS 24 * 2

enum BotState { WelcomeState = 0, ConfigState = 1, RunState = 2 };

const char *lcdMessageWelcome = "WELCOME         \0.RUN     CONFIG.";
const char *lcdMessageRun = "RUN             \0.       WELCOME.";
const char *lcdMessageConfig = "CONFIG          \0.WELCOME     ++.";

class Bot {

public:
  BotState state;
  uint32_t waterPeriodHours;
  const char *lcdMessage;
  void (*lcdWrite)(const char *);

  Bot(void (*l)(const char *)) {
    this->state = WelcomeState;
    this->waterPeriodHours = DEFAULT_WATER_PERIOD_HOURS;
    this->lcdWrite = l;
  }

  void run(bool button0Pressed, bool button1Pressed, bool timerInterrupt) {

    switch (this->state) {

    case RunState:
      if (button1Pressed) {
        toDisplayState();
      } else if (timerInterrupt) {
        goWater();
        toRunState();
      }
      break;
    case WelcomeState:
      if (button0Pressed) {
        toRunState();
      } else if (button1Pressed) {
        toConfigState();
      }
      break;
    case ConfigState:
      if (button0Pressed) {
        toDisplayState();
      } else if (button1Pressed) {
        increaseWaterPeriod();
        toConfigState();
      }
      break;
    default:
      break;
    }
  }

private:
  void toDisplayState() {
    this->lcdWrite(lcdMessageWelcome);
    setState(WelcomeState);
  }

  void toRunState() {
    this->lcdWrite(lcdMessageRun);
    setState(RunState);
  }

  void toConfigState() {
    this->lcdWrite(lcdMessageConfig);
    setState(ConfigState);
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
