#ifndef CLOCK_INC
#define CLOCK_INC

#include <Log.h>
#include <Misc.h>

#define SECONDS_IN_DAY (SECONDS_IN_HOUR * 24)
#define SECONDS_IN_HOUR 3600L
#define SECONDS_IN_MINUTE 60

#ifdef CYCLE_OF_1S
#define INTERNAL_CYCLE_TO_SECONDS_FACTOR 1.024f
#else // CYCLE_OF_8S
#define INTERNAL_CYCLE_TO_SECONDS_FACTOR 8.192f
#endif

#define CYCLES_IN_30_DAYS ((SECONDS_IN_HOUR * 24 * 30) / INTERNAL_CYCLE_TO_SECONDS_FACTOR)

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
  int matchInvalidateCounter;

  bool matches(int day, int hour, int minute);
  bool isValidMatch();
  long getSecondsFromT0();
  void invalidateFollowingMatches();

public:
  Clock(int numberOfActors);

  bool matches(int freqIndex);
  void cycle();
  void setFrequency(int freqIndex, Frequency f);
  void setNextFrequency(int freqIndex);
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
