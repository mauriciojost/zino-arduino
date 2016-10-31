#ifndef CLOCK_INC
#define CLOCK_INC

#include <Log.h>
#include <Misc.h>

#define SECONDS_IN_DAY (SECONDS_IN_HOUR * 24)
#define SECONDS_IN_HOUR 3600L
#define SECONDS_IN_MINUTE 60

#define CYCLE_TO_SECONDS_FACTOR_INCR 0.001f
#define CYCLE_TO_SECONDS_FACTOR_MAX 1.250f
#define CYCLE_TO_SECONDS_FACTOR_MIN 1.000f

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
  DelimiterAmountOfFrequencies
};

class Clock {

private:
  Frequency *freqs;
  double cyclesFromT0;
  double cycleToSecondsFactor;
  int *matchInvalidateCounters;
  int nroActors;

  bool matches(int day, int hour, int minute);
  bool isValidMatch(int index);
  long getSecondsFromT0();
  void invalidateFollowingMatches(int index);

public:
  Clock(int numberOfActors, double cycleSecondFactor);

  bool matches(int freqIndex);
  void cycle();
  void setFrequency(int freqIndex, Frequency f);
  void setNextFrequency(int freqIndex);
  void increaseFactor();
  double getFactor();
  void set(int days, int hours, int minutes, int seconds);
  const char *getFrequencyDescription(int freqIndex);
  int getDays();
  int getHours();
  int getMinutes();
  int getSeconds();
  void increaseHour();
  void increaseMinute();
  void getTimeAsString(char *buffer);
};

#endif // CLOCK_INC
