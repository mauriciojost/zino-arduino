#include <Clock.h>

#define INVALIDATE_PERIOD_CYCLES ((int)(100.0f / INTERNAL_CYCLE_TO_SECONDS_FACTOR)) // 100 seconds

#define ONCE_H 24
#define ONCE_M 60

#define ANY 1111

const char *frequencies[DelimiterAmountOfFrequencies] = {"1/MONTH", "2/MONTH", "1/WEEK", "2/WEEK", "3/WEEK",
                                                         "1/DAY",   "2/DAY",   "1/HOUR", "2/HOUR", "1/5MINUTES"};

// PUBLIC

Clock::Clock(int numberOfActors) {
  set(0, 0, 0, 0);
  matchInvalidateCounter = 0;
  freqs = new Frequency[numberOfActors];
  for (int i = 0; i < numberOfActors; i++) {
    freqs[i] = OncePerDay, OncePerDay;
  }
}

bool Clock::matches(int freqIndex) {
  bool timeMatches = false;
  switch (freqs[freqIndex]) {
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
      log(Info, "TMATCH!!!!!", freqIndex);
      invalidateFollowingMatches();
      return true;
    } else {
      log(Info, "NOTMATCH (BOUNCING)", freqIndex);
      return false;
    }
  } else {
    log(Info, "NOTMATCH", freqIndex);
    return false;
  }
}

void Clock::cycle() {
  cyclesFromT0 = rollValue(cyclesFromT0 + 1.0, 0.0, (double)CYCLES_IN_30_DAYS);
  matchInvalidateCounter = constrainValue(matchInvalidateCounter - 1, 0, INVALIDATE_PERIOD_CYCLES);
  log(Info, "TICK ", (int)cyclesFromT0);
}

void Clock::setFrequency(int i, Frequency f) { freqs[i] = f; }

void Clock::setNextFrequency(int i) { freqs[i] = (Frequency)((freqs[i] + 1) % DelimiterAmountOfFrequencies); }

void Clock::set(int days, int hours, int minutes, int seconds) {
  double secondsFromT0 = days * SECONDS_IN_DAY + hours * SECONDS_IN_HOUR + minutes * SECONDS_IN_MINUTE + seconds;
  cyclesFromT0 = secondsFromT0 / INTERNAL_CYCLE_TO_SECONDS_FACTOR;
}

const char *Clock::getFrequencyDescription(int i) { return frequencies[freqs[i]]; }

int Clock::getDays() { return getSecondsFromT0() / SECONDS_IN_DAY; }

int Clock::getHours() { return (getSecondsFromT0() % SECONDS_IN_DAY) / SECONDS_IN_HOUR; }

int Clock::getMinutes() { return (getSecondsFromT0() % SECONDS_IN_HOUR) / SECONDS_IN_MINUTE; }

int Clock::getSeconds() { return getSecondsFromT0() % SECONDS_IN_MINUTE; }

void Clock::increaseHour() {
  int h = getHours();
  int m = getMinutes();
  int nh = rollValue(h + 1, 0, 23);
  log(Debug, "H:", (int)h);
  log(Debug, "M:", (int)m);
  log(Debug, "NH:", (int)nh);
  set(0, nh, m, 0);
}

void Clock::increaseMinute() {
  int h = getHours();
  int m = getMinutes();
  int nm = rollValue(m + 1, 0, 59);
  log(Debug, "H:", (int)h);
  log(Debug, "M:", (int)m);
  log(Debug, "NM:", (int)nm);
  set(0, h, nm, 0);
}

void Clock::getTimeAsString(char *buffer) {
  int h = getHours();
  int m = getMinutes();
  bool am = h < 12;
  int nh = (h<13?h:h-12A);
  if (am) {
    sprintf(buffer, "%02d:%02d AM", nh, m);
  } else {
    sprintf(buffer, "%02d:%02d PM", nh, m);
  }
}

// PRIVATE

bool Clock::matches(int day, int hour, int minute) {
  bool matchesDays = ((getDays() % day) == 0) || (day == ANY);
  bool matchesHours = ((getHours() % hour) == 0) || (hour == ANY);
  bool matchesMinutes = ((getMinutes() % minute) == 0) || (minute == ANY);
  bool allMatch = matchesDays && matchesHours && matchesMinutes;
  if (allMatch) {
    log(Debug, "WTIME:");
    log(Debug, " d:", getDays());
    log(Debug, " h:", getHours());
    log(Debug, " m:", getMinutes());
  }
  return allMatch;
}

bool Clock::isValidMatch() { return matchInvalidateCounter != 0; }

long Clock::getSecondsFromT0() {
  double secFromMidnight = (cyclesFromT0 * INTERNAL_CYCLE_TO_SECONDS_FACTOR);
  return (long)round(secFromMidnight);
}

void Clock::invalidateFollowingMatches() { matchInvalidateCounter = INVALIDATE_PERIOD_CYCLES; }
