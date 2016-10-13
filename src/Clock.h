#ifndef CLOCK_INC
#define CLOCK_INC

#include <Log.h>
#include <Misc.h>

#define SECONDS_IN_DAY (SECONDS_IN_HOUR * 24)
#define SECONDS_IN_HOUR ((unsigned long)3600)
#define SECONDS_IN_MINUTE 60

#ifdef CYCLE_OF_1S
#define INTERNAL_CYCLE_TO_SECONDS_FACTOR 1.024f
#else
#define INTERNAL_CYCLE_TO_SECONDS_FACTOR 8.192f
#endif // CYCLE_OF_8S

#define CYCLES_IN_30_DAYS                                                      \
  ((SECONDS_IN_HOUR * 24 * 30) / INTERNAL_CYCLE_TO_SECONDS_FACTOR)

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

private:
  Frequency freq;
  double cyclesFromT0;
  int matchInvalidateCounter;

  bool matches(unsigned int day, unsigned int hour, unsigned int minute);
  bool isValidMatch();
  unsigned long getSecondsFromT0();
  void invalidateFollowingMatches();

public:
  Clock();

  bool matches();
  void cycle();
  void setFrequency(Frequency f);
  void setNextFrequency();
  void set(unsigned int days, unsigned int hours, unsigned int minutes,
           unsigned int seconds);
  const char *getFrequencyDescription();
  unsigned int getDays();
  unsigned int getHours();
  unsigned int getMinutes();
  unsigned int getSeconds();
  void increaseHour();
  void increaseMinute();
  void getTimeString(char * buffer);
};

#endif // CLOCK_INC
