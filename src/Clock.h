#include <Debug.h>
#include <Misc.h>

#define INTERNAL_CYCLE_TO_SECONDS_FACTOR 8.0f
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

const char * frequencies[DelimiterAmountOfFrequencies] = {
  "1/MONTH",
  "2/MONTH",
  "1/WEEK",
  "2/WEEK",
  "3/WEEK",
  "1/DAY",
  "2/DAY",
  "1/HOUR",
  "2/HOUR",
  "1/5MINUTES"
};

class Clock {

public:

  Clock() {
    this->set(0, 0, 0, 0);
    this->ticksBeforeEnablingWatering = 0;
    this->freq = OncePerDay;
  }

  bool isTimeToWater() {
    bool timeToWater = false;
    switch (this->freq) {
      case OncePerMonth:      timeToWater = isTime(30, ONCE_H, ONCE_M); break;
      case TwicePerMonth:     timeToWater = isTime(15, ONCE_H, ONCE_M); break;
      case OncePerWeek:       timeToWater = isTime(07, ONCE_H, ONCE_M); break;
      case TwicePerWeek:      timeToWater = isTime(03, ONCE_H, ONCE_M); break;
      case ThreeTimesPerWeek: timeToWater = isTime(02, ONCE_H, ONCE_M); break;
      case OncePerDay:        timeToWater = isTime(ANY,ONCE_H, ONCE_M); break;
      case TwicePerDay:       timeToWater = isTime(ANY,    12, ONCE_M); break;
      case OncePerHour:       timeToWater = isTime(ANY,     1, ONCE_M); break;
      case TwicePerHour:      timeToWater = isTime(ANY,   ANY,     30); break;
      case OnceEvery5Minutes: timeToWater = isTime(ANY,   ANY,      5); break;
      default: timeToWater = false; break;
    }

    /*
    if (timeToWater){
      debug("TIME TO WATER");
      debug(this->ticksBeforeEnablingWatering);
      debug((int)timeToWater);
    }
    */
    if (timeToWater && !isABouncing()) {
      enableAntiBouncing();
      return true;
    } else {
      return false;
    }
  }

  bool isABouncing() {
    return this->ticksBeforeEnablingWatering != 0;
  }
  void enableAntiBouncing() {
      this->ticksBeforeEnablingWatering = DEBOUNCING_WATERING_TICKS;
  }

  void disableAntiBouncing() {
      this->ticksBeforeEnablingWatering = 0;
  }
  void tickForAntiBouncing() {
    this->ticksBeforeEnablingWatering = constrainValue(this->ticksBeforeEnablingWatering - 1, 0, 10);
  }


  const char* getFrequencyDescription() {
    return frequencies[this->freq];
  }

  void tick() {
    this->cyclesFromMidnight = rollValue(this->cyclesFromMidnight + 1, 0, CYCLES_IN_30_DAYS);
    tickForAntiBouncing();

    /*
    debug("---");
    debug(this->cyclesFromMidnight);
    debug(this->ticksBeforeEnablingWatering);
    debug(this->getSecondsFromMidnight());
    debug("---");
    debug(this->getDays());
    debug(this->getHours());
    debug(this->getMinutes());
    debug("---");
    */
  }

  void setFrequency(Frequency f) {
    this->freq = f;
  }

  void setNextFrequency() {
    this->freq = (Frequency)((this->freq + 1) % DelimiterAmountOfFrequencies);
  }

  void set(unsigned int days, unsigned int hours, unsigned int minutes, unsigned int seconds) {
    unsigned long secondsFromMidnight = days * SECONDS_IN_DAY + hours * SECONDS_IN_HOUR + minutes * SECONDS_IN_MINUTE + seconds;
    this->cyclesFromMidnight = secondsFromMidnight / INTERNAL_CYCLE_TO_SECONDS_FACTOR;
    this->secondsOffset = secondsFromMidnight - this->cyclesFromMidnight * INTERNAL_CYCLE_TO_SECONDS_FACTOR;
  }

  unsigned int getDays() {
    return this->getSecondsFromMidnight() / SECONDS_IN_DAY;
  }

  unsigned int getHours() {
    return (this->getSecondsFromMidnight() % SECONDS_IN_DAY) / SECONDS_IN_HOUR;
  }

  unsigned int getMinutes() {
    return (this->getSecondsFromMidnight() % SECONDS_IN_HOUR) / SECONDS_IN_MINUTE ;
  }

  unsigned int getSeconds() {
    return this->getSecondsFromMidnight() % SECONDS_IN_MINUTE;
  }

private:

  unsigned int secondsOffset;
  unsigned long cyclesFromMidnight;
  Frequency freq;
  int ticksBeforeEnablingWatering;

  bool isTime(unsigned int day, unsigned int hour, unsigned int minute) {
    bool matchesDays =  ((this->getDays() % day) == 0)       || (day == ANY);
    bool matchesHours = ((this->getHours() % hour) == 0)     || (hour == ANY);
    bool matchesMinutes = ((this->getMinutes() % minute) == 0) || (minute == ANY);
    bool allMatch = matchesDays && matchesHours && matchesMinutes;
    /*
    if (allMatch) {
      debug("WTIME AT");
      debug(this->getDays());
      debug(this->getHours());
      debug(this->getMinutes());
    }
    */
    return allMatch;
  }

  unsigned long getSecondsFromMidnight() {
    return this->cyclesFromMidnight * INTERNAL_CYCLE_TO_SECONDS_FACTOR + this->secondsOffset;
  }

};
