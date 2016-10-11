#include <Clock.h>

// Ratio between cycles and seconds
#define INTERNAL_CYCLE_TO_SECONDS_FACTOR 8.192f

#define DEBOUNCING_WATERING_CYCLES 10

//
#define SECONDS_IN_DAY (SECONDS_IN_HOUR * 24)
#define SECONDS_IN_HOUR ((unsigned long)3600)
#define SECONDS_IN_MINUTE 60

#define ONCE_H 24
#define ONCE_M 60

#define CYCLES_IN_30_DAYS ((SECONDS_IN_HOUR * 24 * 30) / INTERNAL_CYCLE_TO_SECONDS_FACTOR)

#define ANY 1111

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
  this->matchInvalidateCounter = 0;
  this->freq = OncePerDay;
}

bool Clock::matches() {
  bool timeMatches = false;
  switch (this->freq) {
    case OncePerMonth:      timeMatches = matches(30, ONCE_H, ONCE_M); break;
    case TwicePerMonth:     timeMatches = matches(15, ONCE_H, ONCE_M); break;
    case OncePerWeek:       timeMatches = matches(07, ONCE_H, ONCE_M); break;
    case TwicePerWeek:      timeMatches = matches(03, ONCE_H, ONCE_M); break;
    case ThreeTimesPerWeek: timeMatches = matches(02, ONCE_H, ONCE_M); break;
    case OncePerDay:        timeMatches = matches(ANY,ONCE_H, ONCE_M); break;
    case TwicePerDay:       timeMatches = matches(ANY,    12, ONCE_M); break;
    case OncePerHour:       timeMatches = matches(ANY,     1, ONCE_M); break;
    case TwicePerHour:      timeMatches = matches(ANY,   ANY,     30); break;
    case OnceEvery5Minutes: timeMatches = matches(ANY,   ANY,      5); break;
    default: timeMatches = false; break;
  }

  if (timeMatches) {
    if (!validMatch()) {
      log(Info, "WAT!");
      invalidateFollowingMatches();
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

// PRIVATE

bool Clock::matches(unsigned int day, unsigned int hour, unsigned int minute) {
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


bool Clock::validMatch() {
  return this->matchInvalidateCounter != 0;
}

void Clock::invalidateFollowingMatches() {
  this->matchInvalidateCounter = DEBOUNCING_WATERING_CYCLES;
}

const char* Clock::getFrequencyDescription() {
  return frequencies[this->freq];
}

void Clock::cycle() {
  this->cyclesFromMidnight = rollValue(this->cyclesFromMidnight + 1, 0, CYCLES_IN_30_DAYS);
  this->matchInvalidateCounter = constrainValue(this->matchInvalidateCounter - 1, 0, 10);
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

unsigned long Clock::getSecondsFromMidnight() {
  double secFromMidnight = (this->cyclesFromMidnight * INTERNAL_CYCLE_TO_SECONDS_FACTOR);
  return (unsigned long) round(secFromMidnight);
}
