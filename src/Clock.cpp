#include <Clock.h>

#define INVALIDATE_PERIOD_CYCLES                                               \
  ((int)(100.0f / INTERNAL_CYCLE_TO_SECONDS_FACTOR)) // 100 seconds

#define ONCE_H 24
#define ONCE_M 60

#define ANY 1111

const char *frequencies[DelimiterAmountOfFrequencies] = {
    "1/MONTH", "2/MONTH", "1/WEEK", "2/WEEK", "3/WEEK",
    "1/DAY",   "2/DAY",   "1/HOUR", "2/HOUR", "1/5MINUTES"};

// PUBLIC

Clock::Clock() {
  this->set(0, 0, 0, 0);
  this->matchInvalidateCounter = 0;
  this->freq = OncePerDay;
}

bool Clock::matches() {
  bool timeMatches = false;
  switch (this->freq) {
  case OncePerMonth:
    timeMatches = matches(30, ONCE_H, ONCE_M);
    break;
  case TwicePerMonth:
    timeMatches = matches(15, ONCE_H, ONCE_M);
    break;
  case OncePerWeek:
    timeMatches = matches(07, ONCE_H, ONCE_M);
    break;
  case TwicePerWeek:
    timeMatches = matches(03, ONCE_H, ONCE_M);
    break;
  case ThreeTimesPerWeek:
    timeMatches = matches(02, ONCE_H, ONCE_M);
    break;
  case OncePerDay:
    timeMatches = matches(ANY, ONCE_H, ONCE_M);
    break;
  case TwicePerDay:
    timeMatches = matches(ANY, 12, ONCE_M);
    break;
  case OncePerHour:
    timeMatches = matches(ANY, 1, ONCE_M);
    break;
  case TwicePerHour:
    timeMatches = matches(ANY, ANY, 30);
    break;
  case OnceEvery5Minutes:
    timeMatches = matches(ANY, ANY, 5);
    break;
  default:
    timeMatches = false;
    break;
  }

  if (timeMatches) {
    if (!isValidMatch()) {
      log(Info, "TMATCH!!!!!");
      invalidateFollowingMatches();
      return true;
    } else {
      log(Info, "NOTMATCH (BOUNCING)", this->matchInvalidateCounter);
      return false;
    }
  } else {
    log(Info, "NOTMATCH");
    return false;
  }
}

void Clock::cycle() {
  this->cyclesFromT0 = rollValue(this->cyclesFromT0 + 1, 0, CYCLES_IN_30_DAYS);
  this->matchInvalidateCounter = constrainValue(
      this->matchInvalidateCounter - 1, 0, INVALIDATE_PERIOD_CYCLES);
  log(Info, "TICK ", (int)this->cyclesFromT0);
}

void Clock::setFrequency(Frequency f) { this->freq = f; }

void Clock::setNextFrequency() {
  this->freq = (Frequency)((this->freq + 1) % DelimiterAmountOfFrequencies);
}

void Clock::set(int days, int hours, int minutes, int seconds) {
  double secondsFromT0 = days * SECONDS_IN_DAY + hours * SECONDS_IN_HOUR + minutes * SECONDS_IN_MINUTE + seconds;
  this->cyclesFromT0 = secondsFromT0 / INTERNAL_CYCLE_TO_SECONDS_FACTOR;
}

const char *Clock::getFrequencyDescription() { return frequencies[this->freq]; }

int Clock::getDays() { return this->getSecondsFromT0() / SECONDS_IN_DAY; }

int Clock::getHours() {
  return (this->getSecondsFromT0() % SECONDS_IN_DAY) / SECONDS_IN_HOUR;
}

int Clock::getMinutes() {
  return (this->getSecondsFromT0() % SECONDS_IN_HOUR) / SECONDS_IN_MINUTE;
}

int Clock::getSeconds() { return this->getSecondsFromT0() % SECONDS_IN_MINUTE; }

void Clock::increaseHour() {
  int h = this->getHours();
  int m = this->getMinutes();
  int nh = rollValue(h + 1, 0, 24);
  log(Debug, "H:", (int)h);
  log(Debug, "M:", (int)m);
  log(Debug, "NH:", (int)nh);
  this->set(0, nh, m, 0);
}

void Clock::increaseMinute() {
  int h = this->getHours();
  int m = this->getMinutes();
  int nm = rollValue(m + 1, 0, 60);
  log(Debug, "H:", (int)h);
  log(Debug, "M:", (int)m);
  log(Debug, "NM:", (int)nm);
  this->set(0, h, nm, 0);
}

void Clock::getTimeString(char *buffer) {
  sprintf(buffer, "%02d:%02d", (int)(this->getHours()),
          (int)(this->getMinutes()));
}

// PRIVATE

bool Clock::matches(int day, int hour, int minute) {
  bool matchesDays = ((this->getDays() % day) == 0) || (day == ANY);
  bool matchesHours = ((this->getHours() % hour) == 0) || (hour == ANY);
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

bool Clock::isValidMatch() { return this->matchInvalidateCounter != 0; }

long Clock::getSecondsFromT0() {
  double secFromMidnight =
      (this->cyclesFromT0 * INTERNAL_CYCLE_TO_SECONDS_FACTOR);
  return (long)round(secFromMidnight);
}

void Clock::invalidateFollowingMatches() {
  this->matchInvalidateCounter = INVALIDATE_PERIOD_CYCLES;
}
