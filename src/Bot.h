#define constrainValue(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

#define MIN_WATER_PERIOD_MINUTES 1
#define DEFAULT_WATER_PERIOD_MINUTES 60 * 24 * 2
#define MAX_WATER_PERIOD_MINUTES 60 * 24 * 15

enum BotState { WelcomeState = 0, ConfigState = 1, RunState = 2 };

const char* lcdMessageWelcomeUp = "WELCOME        ";
const char* lcdMessageWelcomeDo = "WELCOME DOWN   ";

const char* lcdMessageRunUp = "RUN            ";
const char* lcdMessageRunDo = "RUN DOWN       ";

const char* lcdMessageConfigUp = "CONFIG         ";
const char* lcdMessageConfigDo = "CONFIG     DOWN";

const char* lcdMessageNull = "";

class Bot {

public:
  BotState state;
  uint32_t waterPeriodMinutes;
  uint32_t waterCounter;
  void (*stdOutWriteString)(const char *, const char *);
  void (*stdOutSetCursor)(int, int);

  Bot(void (*wrSt)(const char *, const char *)) {
    this->state = WelcomeState;
    this->waterPeriodMinutes = DEFAULT_WATER_PERIOD_MINUTES;
    this->stdOutWriteString = wrSt;
  }

  void run(bool button0Pressed, bool button1Pressed, bool timerInterrupt) {

    switch (this->state) {

    case RunState:
      if (button1Pressed) {
        toWelcomeState();
      } else if (timerInterrupt) {
        goWater();
        toRunState();
      } else {
        toRunState(); // Might need to remove it
      }
      break;
    case WelcomeState:
      if (button0Pressed) {
        toRunState();
      } else if (button1Pressed) {
        toConfigState();
      } else {
        toWelcomeState(); // Might need to remove it
      }
      break;
    case ConfigState:
      if (button0Pressed) {
        toWelcomeState();
      } else if (button1Pressed) {
        increaseWaterPeriod();
        toConfigState();
      } else {
        toConfigState(); // Might need to remove it
      }
      break;
    default:
      break;
    }
  }

private:
  void toWelcomeState() {
    setState(WelcomeState);
    this->stdOutWriteString(lcdMessageWelcomeUp, lcdMessageWelcomeDo);
  }

  void toRunState() {
    setState(RunState);
    this->stdOutWriteString(lcdMessageRunUp, lcdMessageRunDo);
  }

  void toConfigState() {
    setState(ConfigState);
    this->stdOutWriteString(lcdMessageConfigUp, lcdMessageConfigDo);
  }

  void goWater() {
    // do the watering
  }

  void increaseWaterPeriod() {
    this->waterPeriodMinutes =
        constrainValue(this->waterPeriodMinutes + 1, MIN_WATER_PERIOD_MINUTES, MAX_WATER_PERIOD_MINUTES);
  }

  void setState(BotState newState) { this->state = newState; }

};
