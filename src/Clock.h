#include <Debug.h>
#include <Misc.h>

#define INTERNAL_CYCLE_TO_SECONDS_FACTOR 8.0f

#define SECONDS_IN_DAY (SECONDS_IN_HOUR * 24)
#define SECONDS_IN_HOUR 3600
#define SECONDS_IN_MINUTE 60

enum Frequency {
  OncePerMonth = 0,
  TwicePerMonth = 1,
  OncePerWeek = 2,
  TwicePerWeek = 3,
  ThreeTimesPerWeek = 4,
  OncePerDay = 5,
  TwicePerDayPerDay = 6,
};

class Clock {

public:
  uint8_t secondsOffset;
  uint32_t cyclesFromMidnight;

  Clock() {
    this->set(0, 0, 0);
  }

  void tick() {
    this->cyclesFromMidnight++;
  }

  void set(uint32_t hours, uint32_t minutes, uint32_t seconds) {
    uint32_t secondsFromMidnight = hours * SECONDS_IN_HOUR + minutes * SECONDS_IN_MINUTE + seconds;
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
