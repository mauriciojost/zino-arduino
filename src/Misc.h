#ifndef MISC_LIB
#define MISC_LIB

#define constrainValue(amt, low, high)                                         \
  ((amt) < (low) ? (low) : ((amt) > (high) ? (high) : (amt)))

#define rollValue(amt, low, high)                                         \
  ((amt) < (low) ? (low) : ((amt) > (high) ? (low) : (amt)))

void toTimeString(uint32_t seconds, char* msg) {
  int sec = seconds % 60;
  int min = (seconds / 60) % 60;
  int hou = (seconds / 3600);
  sprintf(msg, "%02d:%02d:%02d", hou, min, sec);
}

#endif

