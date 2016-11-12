#ifndef CLOCK_INC
#define CLOCK_INC

#include <Log.h>
#include <Misc.h>

#define SECONDS_IN_DAY (SECONDS_IN_HOUR * 24)
#define SECONDS_IN_HOUR 3600L
#define SECONDS_IN_MINUTE 60

#ifdef CYCLE_OF_1S
#define SECS_CYCLE_FACTOR_DEFAULT 1.020f
#define SECS_CYCLE_FACTOR_INCR 0.0001f
#define SECS_CYCLE_FACTOR_MAX 1.200f
#define SECS_CYCLE_FACTOR_MIN 1.000f
#else // CYCLE_OF_8S
#define SECS_CYCLE_FACTOR_DEFAULT 8.210f
#define SECS_CYCLE_FACTOR_INCR 0.001f
#define SECS_CYCLE_FACTOR_MAX 8.500f
#define SECS_CYCLE_FACTOR_MIN 8.000f
#endif

enum Frequency {
  OncePerMonth = 0,
  TwicePerMonth,
  OncePerWeek,
  TwicePerWeek,
  ThreeTimesPerWeek,
  OncePerDay,
  TwicePerDay,
  OncePerHour,
  TwicePerHour,
  OnceEvery5Minutes,
  OnceEvery2Minutes,
  DelimiterAmountOfFrequencies
};

class Clock {

private:
  long t0; // amount of seconds from midnight as set up by the user
  float cyclesFromT0; // amount of cycles passed from t0
  float secToCyclesFactor; // ratio seconds / cycle factor (normally bigger than 1)

  int nroActors; // amount of actors
  Frequency *freqs; // array whose elements are the frequencies associated to each actor
  int *matchInvalidateCounters; // array whose elements are counters that allow to invalidate matches associated to each actor

  bool matches(int day, int hour, int minute);
  bool isValidMatch(int index);
  void invalidateFollowingMatches(int index);

public:
  Clock(int numberOfActors, float secCycFactor);

  bool matches(int freqIndex);
  void cycle();
  void setFrequency(int freqIndex, Frequency f);
  void setNextFrequency(int freqIndex);
  void increaseFactor();
  float getFactor();
  void set(int days, int hours, int minutes, int seconds);
  const char *getFrequencyDescription(int freqIndex);
  int getDays();
  int getHours();
  int getMinutes();
  int getSeconds();
  void increaseHour();
  void increaseMinute();
  void populateWithTime(char *buffer);
  long getSecondsFromT0();
  long getCyclesFromT0();

};

#endif // CLOCK_INC
