#ifndef CLOCK_INC
#define CLOCK_INC

#include <Log.h>
#include <Misc.h>

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

  double cyclesFromMidnight;
  Frequency freq;
  int matchInvalidateCounter;

  bool matches(unsigned int day, unsigned int hour, unsigned int minute);
  bool validMatch();
  unsigned long getSecondsFromMidnight();
  void invalidateFollowingMatches();
  void disableAntiBouncing();

public:

  Clock();

  bool matches();
  void cycle();
  void setFrequency(Frequency f);
  void setNextFrequency();
  void set(unsigned int days, unsigned int hours, unsigned int minutes, unsigned int seconds);
  const char* getFrequencyDescription();
  unsigned int getDays();
  unsigned int getHours();
  unsigned int getMinutes();
  unsigned int getSeconds();

};

#endif // CLOCK_INC
