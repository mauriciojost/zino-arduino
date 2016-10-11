#ifndef CLOCK_INC
#define CLOCK_INC

#include <Log.h>
#include <Misc.h>

#define INTERNAL_CYCLE_TO_SECONDS_FACTOR 8.192f
#define DEBOUNCING_WATERING_TICKS 10

#define SECONDS_IN_DAY (SECONDS_IN_HOUR * 24)
#define SECONDS_IN_HOUR ((unsigned long)3600)
#define SECONDS_IN_MINUTE 60

#define ONCE_H 24
#define ONCE_M 60

#define CYCLES_IN_30_DAYS ((SECONDS_IN_HOUR * 24 * 30) / INTERNAL_CYCLE_TO_SECONDS_FACTOR)

#define ANY 1111

enum Frequency {
  OncePerMonth = 0,
  TwicePerMonth = 1,
  OncePerWeek = 2,
  TwicePerWeek = 3,
  ThreeTimesPerWeek = 4,
  OncePerDay = 5,
  TwicePerDay = 6,
  OncePerHour = 7,
  TwicePerHour = 8,
  OnceEvery5Minutes = 9,
  DelimiterAmountOfFrequencies = 10
};

class Clock {

public:

  Clock();

  bool isTimeToWater();

  bool isABouncing();
  void enableAntiBouncing();
  void disableAntiBouncing();
  void tickForAntiBouncing();
  const char* getFrequencyDescription();
  void tick();
  void setFrequency(Frequency f);
  void setNextFrequency();
  void set(unsigned int days, unsigned int hours, unsigned int minutes, unsigned int seconds);

  unsigned int getDays();
  unsigned int getHours();
  unsigned int getMinutes();
  unsigned int getSeconds();

private:

  double cyclesFromMidnight;
  Frequency freq;
  int ticksBeforeEnablingWatering;

  bool isTime(unsigned int day, unsigned int hour, unsigned int minute);
  unsigned long getSecondsFromMidnight();

};

#endif // CLOCK_INC
