#include <Clock.h>

#define INVALIDATE_PERIOD_SECONDS 119.0f

#define ONCE_H 24
#define ONCE_M 60

#define ANY 1111

const char *frequencies[DelimiterAmountOfFrequencies] =
    {"1/month", "2/month", "1/week", "2/week", "3/week", "1/day", "2/day", "1/hour", "2/hour", "1/5min", "1/2min"};

Clock::Clock(int numberOfActors, float ctsf) {
  set(0, 0, 0, 0);
  freqs = new Frequency[numberOfActors];
  matchInvalidateCounters = new int[numberOfActors];
  nroActors = numberOfActors;
  secToCyclesFactor = ctsf;
  for (int i = 0; i < numberOfActors; i++) {
    freqs[i] = OncePerDay;
    matchInvalidateCounters[i] = 0;
  }
}

bool Clock::matches(int index) {
  bool timeMatches = false;
  Frequency freq = freqs[index];
  log(Info, "  CLK FREQ: ", frequencies[freq]);
  switch (freq) {
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
    case OnceEvery2Minutes:
      timeMatches = matches(ANY, ANY, 2);
      break;
    default:
      timeMatches = false;
      break;
  }

  if (timeMatches) {
    if (isValidMatch(index)) {
      log(Info, "  CLK MATCH");
      invalidateFollowingMatches(index);
      return true;
    } else {
      log(Info, "  CLK (MUTE) FOR ", matchInvalidateCounters[index]);
      return false;
    }
  } else {
    log(Info, "  CLK ZZZ");
    return false;
  }
}

void Clock::cycle() {
  long cyclesIn30Days = ((SECONDS_IN_HOUR * 24 * 30) / secToCyclesFactor);
  cyclesFromT0 = rollValue(cyclesFromT0 + 1, 0, cyclesIn30Days);
  for (int i = 0; i < nroActors; i++) {
    matchInvalidateCounters[i] = constrainValue(matchInvalidateCounters[i] - 1, 0, (int)(INVALIDATE_PERIOD_SECONDS / secToCyclesFactor));
  }
  log(Info, "TICK ", (int)cyclesFromT0);
}

void Clock::setFrequency(int i, Frequency f) {
  freqs[i] = f;
}

void Clock::setNextFrequency(int i) {
  freqs[i] = (Frequency)((freqs[i] + 1) % DelimiterAmountOfFrequencies);
}

void Clock::set(int days, int hours, int minutes, int seconds) {
  t0 = days * SECONDS_IN_DAY + hours * SECONDS_IN_HOUR + minutes * SECONDS_IN_MINUTE + seconds;
  cyclesFromT0 = 0;
}

const char *Clock::getFrequencyDescription(int i) {
  return frequencies[freqs[i]];
}

int Clock::getDays() {
  return getSecondsFromT0() / SECONDS_IN_DAY;
}

int Clock::getHours() {
  return (getSecondsFromT0() % SECONDS_IN_DAY) / SECONDS_IN_HOUR;
}

int Clock::getMinutes() {
  return (getSecondsFromT0() % SECONDS_IN_HOUR) / SECONDS_IN_MINUTE;
}

int Clock::getSeconds() {
  return getSecondsFromT0() % SECONDS_IN_MINUTE;
}

void Clock::increaseFactor() {
  secToCyclesFactor =
      rollValue(secToCyclesFactor + SECS_CYCLE_FACTOR_INCR, SECS_CYCLE_FACTOR_MIN, SECS_CYCLE_FACTOR_MAX);
}

float Clock::getFactor() {
  return secToCyclesFactor;
}

void Clock::increaseHour() {
  int h = getHours();
  int m = getMinutes();
  int nh = rollValue(h + 1, 0, 23);
  set(0, nh, m, 0);
}

void Clock::increaseMinute() {
  int h = getHours();
  int m = getMinutes();
  int nm = rollValue(m + 1, 0, 59);
  set(0, h, nm, 0);
}
void Clock::populateWithTime(char *buffer) {
  int h = getHours();
  int m = getMinutes();
  int s = getSeconds();
  bool am = h < 12;
  int nh = (h < 13 ? h : h - 12);
  if (am) {
    sprintf(buffer, "%02d:%02d:%02d am", nh, m, s);
  } else {
    sprintf(buffer, "%02d:%02d:%02d pm", nh, m, s);
  }
}

bool Clock::matches(int day, int hour, int minute) {
  bool matchesDays = ((getDays() % day) == 0) || (day == ANY);
  bool matchesHours = ((getHours() % hour) == 0) || (hour == ANY);
  bool matchesMinutes = ((getMinutes() % minute) == 0) || (minute == ANY);
  bool allMatch = matchesDays && matchesHours && matchesMinutes;
  return allMatch;
}

bool Clock::isValidMatch(int index) {
  return matchInvalidateCounters[index] <= 0;
}

long Clock::getSecondsFromT0() {
  float secFromT0 = (cyclesFromT0 * secToCyclesFactor);
  return (long)round(secFromT0) + t0;
}

long Clock::getCyclesFromT0() {
  return (long)cyclesFromT0;
}

void Clock::invalidateFollowingMatches(int index) {
  matchInvalidateCounters[index] = (int)(INVALIDATE_PERIOD_SECONDS / secToCyclesFactor);
}
