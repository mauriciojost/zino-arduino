#include <Debug.h>
#include <Misc.h>

#define INTERNAL_CYCLE_TO_SECONDS_FACTOR 8.0f

#define SECONDS_IN_DAY (SECONDS_IN_HOUR * 24)
#define SECONDS_IN_HOUR 3600
#define SECONDS_IN_MINUTE 60

#define ONCE_H 24
#define ONCE_M 60
#define ONCE_S 60

#define ANY 10000

enum Frequency {
  OncePerMonth = 0,
  TwicePerMonth = 1,
  OncePerWeek = 2,
  TwicePerWeek = 3,
  ThreeTimesPerWeek = 4,
  OncePerDay = 5,
  TwicePerDay = 6,
};

class Clock {

public:
  uint8_t secondsOffset;
  uint32_t cyclesFromMidnight;

  bool isTimeToWater(Frequency f) {
    bool ret = false;
    switch (f) {
      case OncePerMonth:      ret = isTime(30, ONCE_H, ONCE_M, ONCE_S); break;
      case TwicePerMonth:     ret = isTime(15, ONCE_H, ONCE_M, ONCE_S); break;
      case OncePerWeek:       ret = isTime(07, ONCE_H, ONCE_M, ONCE_S); break;
      case TwicePerWeek:      ret = isTime(03, ONCE_H, ONCE_M, ONCE_S); break;
      case ThreeTimesPerWeek: ret = isTime(02, ONCE_H, ONCE_M, ONCE_S); break;
      case OncePerDay:        ret = isTime(ANY,ONCE_H, ONCE_M, ONCE_S); break;
      case TwicePerDay:       ret = isTime(ANY,    12, ONCE_M, ONCE_S); break;
      default: ret = false; break;
    }
    return ret;
  }

  bool isTime(uint32_t day, uint32_t hour, uint32_t minute, uint32_t second) {
    bool matchesDays =  ((this->getDays() % day) == 0)       || (day == ANY);
    bool matchesHours = ((this->getHours() % hour) == 0)     || (hour == ANY);
    bool matchesMinutes = ((this->getMinutes() % minute) == 0) || (minute == ANY);
    bool matchesSeconds = ((this->getSeconds() % second) == 0) || (second == ANY);
    bool allMatch = matchesDays && matchesHours && matchesMinutes && matchesSeconds;
    return allMatch;
  }

  Clock() {
    this->set(0, 0, 0, 0);
  }

  void tick() {
    this->cyclesFromMidnight++;
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

  uint32_t getSecondsFromMidnight() {
    return this->cyclesFromMidnight * INTERNAL_CYCLE_TO_SECONDS_FACTOR + this->secondsOffset;
  }

};
