#ifndef UNIT_TEST

// Declare DEBUG to receive logs via serial port

#ifdef DEBUG

#define DELAY_DEBUG_MS 100

void debug(const char *msg) {
  Serial.println(msg);
  delay(DELAY_DEBUG_MS);
}

void debug(int msg) {
  Serial.println(msg);
  delay(DELAY_DEBUG_MS);
}

#else

void debug(const char *msg) {}

void debug(int msg) {}

#endif

#endif
