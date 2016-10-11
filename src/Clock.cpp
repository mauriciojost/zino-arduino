#include <Clock.h>

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

Clock::Clock() {
  this->set(0, 0, 0, 0);
  this->ticksBeforeEnablingWatering = 0;
  this->freq = OncePerDay;
}

bool Clock::isTimeToWater() {
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

  if (timeToWater) {
    if (!isABouncing()) {
      log(Info, "WAT!");
      enableAntiBouncing();
      return true;
    } else {
      log(Info, "!WAT (BNC)");
      return false;
    }
  } else {
    log(Info, "!WAT");
    return false;
  }
}

bool Clock::isABouncing() {
  return this->ticksBeforeEnablingWatering != 0;
}

void Clock::enableAntiBouncing() {
  this->ticksBeforeEnablingWatering = DEBOUNCING_WATERING_TICKS;
}

void Clock::disableAntiBouncing() {
  this->ticksBeforeEnablingWatering = 0;
}

void Clock::tickForAntiBouncing() {
  this->ticksBeforeEnablingWatering = constrainValue(this->ticksBeforeEnablingWatering - 1, 0, 10);
}

const char* Clock::getFrequencyDescription() {
  return frequencies[this->freq];
}

void Clock::tick() {
  this->cyclesFromMidnight = rollValue(this->cyclesFromMidnight + 1, 0, CYCLES_IN_30_DAYS);
  tickForAntiBouncing();
  log(Info, "TICK ", (int)this->cyclesFromMidnight);
}

void Clock::setFrequency(Frequency f) {
  this->freq = f;
}

void Clock::setNextFrequency() {
  this->freq = (Frequency)((this->freq + 1) % DelimiterAmountOfFrequencies);
}

void Clock::set(unsigned int days, unsigned int hours, unsigned int minutes, unsigned int seconds) {
  double secondsFromMidnight = days * SECONDS_IN_DAY + hours * SECONDS_IN_HOUR + minutes * SECONDS_IN_MINUTE + seconds;
  this->cyclesFromMidnight = secondsFromMidnight / INTERNAL_CYCLE_TO_SECONDS_FACTOR;
}

unsigned int Clock::getDays() {
  return this->getSecondsFromMidnight() / SECONDS_IN_DAY;
}

unsigned int Clock::getHours() {
  return (this->getSecondsFromMidnight() % SECONDS_IN_DAY) / SECONDS_IN_HOUR;
}

unsigned int Clock::getMinutes() {
  return (this->getSecondsFromMidnight() % SECONDS_IN_HOUR) / SECONDS_IN_MINUTE ;
}

unsigned int Clock::getSeconds() {
  return this->getSecondsFromMidnight() % SECONDS_IN_MINUTE;
}

bool Clock::isTime(unsigned int day, unsigned int hour, unsigned int minute) {
  bool matchesDays =  ((this->getDays() % day) == 0)       || (day == ANY);
  bool matchesHours = ((this->getHours() % hour) == 0)     || (hour == ANY);
  bool matchesMinutes = ((this->getMinutes() % minute) == 0) || (minute == ANY);
  bool allMatch = matchesDays && matchesHours && matchesMinutes;
  if (allMatch) {
    log(Debug, "WTIME:");
    log(Debug, " d:", this->getDays());
    log(Debug, " h:", this->getHours());
    log(Debug, " m:", this->getMinutes());
  }
  return allMatch;
}

unsigned long Clock::getSecondsFromMidnight() {
  double secFromMidnight = (this->cyclesFromMidnight * INTERNAL_CYCLE_TO_SECONDS_FACTOR);
  return (unsigned long) round(secFromMidnight);
}
