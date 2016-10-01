#define constrainValue(amt, low, high) ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

#define MIN_WATER_PERIOD_MINUTES 1
#define DEFAULT_WATER_PERIOD_MINUTES 60 * 24 * 2
#define MAX_WATER_PERIOD_MINUTES 60 * 24 * 15

enum BotState { WelcomeState = 0, ConfigState = 1, RunState = 2 };

const char *lcdMessageWelcomeUp = "WELCOME         ";
//const char *lcdMessageWelcomeDo = "WELCOME DOWN    ";

const char *lcdMessageRunUp = "RUN             ";
//const char *lcdMessageRunDo = "RUN DOWN        ";

const char *lcdMessageConfigUp = "CONFIG          ";
//const char *lcdMessageConfigDo = "CONFIG      DOWN";

class Bot {

public:
  BotState state;
  uint32_t waterPeriodMinutes;
  uint32_t waterCounter;
  void (*stdOutWriteString)(const char *);
  void (*stdOutWriteInt)(int);
  void (*stdOutSetCursor)(int, int);

  Bot(void (*wrSt)(const char *), void (*wrIn)(int), void (*stCu)(int, int)) {
    this->state = WelcomeState;
    this->waterPeriodMinutes = DEFAULT_WATER_PERIOD_MINUTES;
    this->stdOutWriteString = wrSt;
    this->stdOutWriteInt = wrIn;
    this->stdOutSetCursor = stCu;
  }

  void run(bool button0Pressed, bool button1Pressed, bool timerInterrupt) {

    switch (this->state) {

    case RunState:
      if (button1Pressed) {
        toWelcomeState();
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
        toWelcomeState();
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
  void toWelcomeState() {
    setState(WelcomeState);
    this->stdOutSetCursor(0, 0);
    this->stdOutWriteString(lcdMessageWelcomeUp);
    //this->stdOutSetCursor(0, 1);
    //this->stdOutWriteString(lcdMessageWelcomeDo);
  }

  void toRunState() {
    setState(RunState);
    this->stdOutSetCursor(0, 0);
    this->stdOutWriteString(lcdMessageRunUp);
    //this->stdOutSetCursor(0, 1);
    //this->stdOutWriteString(lcdMessageRunDo);
  }

  void toConfigState() {
    setState(ConfigState);
    this->stdOutSetCursor(0, 0);
    this->stdOutWriteString(lcdMessageConfigUp);
    //this->stdOutSetCursor(0, 1);
    //this->stdOutWriteString(lcdMessageConfigDo);
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
