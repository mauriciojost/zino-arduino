#include <Debug.h>
#include <Misc.h>

#define INTERNAL_CYCLE_TO_SECONDS_FACTOR 8.0f

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
    uint32_t secondsFromMidnight = hours * 3600 + minutes * 60 + seconds;
    this->cyclesFromMidnight = secondsFromMidnight / INTERNAL_CYCLE_TO_SECONDS_FACTOR;
    this->secondsOffset = secondsFromMidnight - this->cyclesFromMidnight * INTERNAL_CYCLE_TO_SECONDS_FACTOR;
  }

  uint32_t getHours() {
    return this->getSecondsFromMidnight() / 3600;
  }

  uint32_t getMinutes() {
    return ((this->getSecondsFromMidnight() % 3600)- this->getSeconds()) / 60 ;
  }

  uint32_t getSeconds() {
    return this->getSecondsFromMidnight() % 60;
  }

  uint32_t getSecondsFromMidnight() {
    return this->cyclesFromMidnight * INTERNAL_CYCLE_TO_SECONDS_FACTOR + this->secondsOffset;
  }

};
