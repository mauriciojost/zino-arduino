#include <Debug.h>
#include <Misc.h>

#define INTERNAL_CYCLE_TO_SECONDS_FACTOR 8.0f
#define DEBOUNCING_WATERING_TICKS 3

#define SECONDS_IN_DAY (SECONDS_IN_HOUR * 24)
#define SECONDS_IN_HOUR 3600
#define SECONDS_IN_MINUTE 60

#define ONCE_H 24
#define ONCE_M 60
#define ONCE_S 60

#define CYCLES_IN_30_DAYS (SECONDS_IN_HOUR * 24 * 30 / INTERNAL_CYCLE_TO_SECONDS_FACTOR)

#define ANY 10000

enum Frequency {
  OncePerMonth = 0,
  TwicePerMonth = 1,
  OncePerWeek = 2,
  TwicePerWeek = 3,
  ThreeTimesPerWeek = 4,
  OncePerDay = 5,
  TwicePerDay = 6,
  DelimiterAmountOfFrequencies = 7
};

const char * frequencies[DelimiterAmountOfFrequencies] = {
  "1/MONTH",
  "2/MONTH",
  "1/WEEK",
  "2/WEEK",
  "3/WEEK",
  "1/DAY",
  "2/DAY"
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
      case OncePerMonth:      timeToWater = isTime(30, ONCE_H, ONCE_M, ONCE_S); break;
      case TwicePerMonth:     timeToWater = isTime(15, ONCE_H, ONCE_M, ONCE_S); break;
      case OncePerWeek:       timeToWater = isTime(07, ONCE_H, ONCE_M, ONCE_S); break;
      case TwicePerWeek:      timeToWater = isTime(03, ONCE_H, ONCE_M, ONCE_S); break;
      case ThreeTimesPerWeek: timeToWater = isTime(02, ONCE_H, ONCE_M, ONCE_S); break;
      case OncePerDay:        timeToWater = isTime(ANY,ONCE_H, ONCE_M, ONCE_S); break;
      case TwicePerDay:       timeToWater = isTime(ANY,    12, ONCE_M, ONCE_S); break;
      default: timeToWater = false; break;
    }

    if (timeToWater && ticksBeforeEnablingWatering == 0) {
      ticksBeforeEnablingWatering = DEBOUNCING_WATERING_TICKS;
      return true;
    } else {
      return false;
    }
  }

  const char* getFrequencyDescription() {
    return frequencies[this->freq];
  }

  void tick() {
    this->cyclesFromMidnight = rollValue(this->cyclesFromMidnight + 1, 0, CYCLES_IN_30_DAYS);
    this->ticksBeforeEnablingWatering = constrainValue(this->ticksBeforeEnablingWatering - 1, 0, 10);
  }

  void setFrequency(Frequency f) {
    this->freq = f;
  }

  void setNextFrequency() {
    this->freq = (Frequency)((this->freq + 1) % DelimiterAmountOfFrequencies);
  }

  void set(uint32_t days, uint32_t hours, uint32_t minutes, uint32_t seconds) {
    uint32_t secondsFromMidnight = days * SECONDS_IN_DAY + hours * SECONDS_IN_HOUR + minutes * SECONDS_IN_MINUTE + seconds;
    this->cyclesFromMidnight = secondsFromMidnight / INTERNAL_CYCLE_TO_SECONDS_FACTOR;
    this->secondsOffset = secondsFromMidnight - this->cyclesFromMidnight * INTERNAL_CYCLE_TO_SECONDS_FACTOR;
  }

  uint32_t getDays() {
    return this->getSecondsFromMidnight() / SECONDS_IN_DAY;
  }

  uint32_t getHours() {
    return (this->getSecondsFromMidnight() % SECONDS_IN_DAY) / SECONDS_IN_HOUR;
  }

  uint32_t getMinutes() {
    return (this->getSecondsFromMidnight() % SECONDS_IN_HOUR) / SECONDS_IN_MINUTE ;
  }

  uint32_t getSeconds() {
    return this->getSecondsFromMidnight() % SECONDS_IN_MINUTE;
  }

private:

  uint8_t secondsOffset;
  uint32_t cyclesFromMidnight;
  Frequency freq;
  uint8_t ticksBeforeEnablingWatering;

  bool isTime(uint32_t day, uint32_t hour, uint32_t minute, uint32_t second) {
    bool matchesDays =  ((this->getDays() % day) == 0)       || (day == ANY);
    bool matchesHours = ((this->getHours() % hour) == 0)     || (hour == ANY);
    bool matchesMinutes = ((this->getMinutes() % minute) == 0) || (minute == ANY);
    bool matchesSeconds = ((this->getSeconds() % second) == 0) || (second == ANY);
    bool allMatch = matchesDays && matchesHours && matchesMinutes && matchesSeconds;
    if (allMatch) {
      debug("WTIME AT");
      debug(this->getDays());
      debug(this->getHours());
      debug(this->getMinutes());
      debug(this->getSeconds());
    }
    return allMatch;
  }

  uint32_t getSecondsFromMidnight() {
    return this->cyclesFromMidnight * INTERNAL_CYCLE_TO_SECONDS_FACTOR + this->secondsOffset;
  }


};
