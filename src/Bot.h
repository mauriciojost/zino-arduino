#ifndef BOT_INC
#define BOT_INC

#include <Log.h>
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
  ServoTestState = 7,
  DelimiterAmountOfBotStates = 8
};

struct BotStateData {
  BotState currentState;
  const char *lcdMessage;
  BotState nextState;
};

class Bot {

private:
  void doTransition(BotState toState, bool modePressed, bool setPressed,
                    bool timerInterrupt);

  void toWelcomeState(BotStateData data, bool modePressed, bool setPressed,
                      bool timerInterrupt);
  void toRunState(BotStateData data, bool modePressed, bool setPressed,
                  bool timerInterrupt);
  void toConfigPeriodState(BotStateData data, bool modePressed, bool setPressed,
                           bool timerInterrupt);
  void toConfigAmountState(BotStateData data, bool modePressed, bool setPressed,
                           bool timerInterrupt);
  void toConfigHourState(BotStateData data, bool modePressed, bool setPressed,
                         bool timerInterrupt);
  void toConfigMinuteState(BotStateData data, bool modePressed, bool setPressed,
                           bool timerInterrupt);
  void toConfigFilledState(BotStateData data, bool modePressed, bool setPressed,
                           bool timerInterrupt);
  void toServoTestState(BotStateData data, bool modePressed, bool setPressed,
                           bool timerInterrupt);

  void waterTimeMaybe();
  void increaseWaterPeriod();
  void increaseWaterAmount();
  void increaseHour();
  void increaseMinute();
  void setToFilled();

public:
  Clock clock;              // bot internal clock
  BotState state;           // state of the bot
  ServoState servoState;    // state of the servo
  float waterAmountPerShot; // expressed in fraction of capacity
  float waterCurrentAmount; // expressed in fraction of capacity remaining
  int servoPosition;        // expressed in degrees
  int maxServoPosition;     // expressed in degrees
  void (*stdOutWriteString)(const char *,
                            const char *); // stdout write callback function

  Bot(void (*wrSt)(const char *, const char *));
  void cycle(bool modePressed, bool setPressed, bool timerInterrupt);
  bool isServoDriven();
};

#endif // BOT_INC
