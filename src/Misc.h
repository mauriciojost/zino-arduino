#ifndef MISC_LIB
#define MISC_LIB

#define constrainValue(amt, low, high)                                         \
  ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

#define rollValue(amt, low, high)                                         \
  ((amt) < (low) ? (low) : ((amt) > (high) ? (low) : (amt)))

void toHourMinuteSecondsString(uint32_t seconds, char* msg) {
  int sec = seconds % 60;
  int min = (seconds / 60) % 60;
  int hou = (seconds / 3600);
  sprintf(msg, "%02d:%02d:%02d", hou, min, sec);
}

void toDayHourMinutesString(uint32_t seconds, char* msg) {
  int min = (seconds / 60) % 60;
  int hou = (seconds / 3600) % 24;
  int day = (seconds / (3600 * 24));
  sprintf(msg, "%d days %02d:%02d", day, hou, min);
}

#endif

