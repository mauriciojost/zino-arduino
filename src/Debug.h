#define DELAY_DEBUG_MS 100

#ifndef DEBUG_INC
#define DEBUG_INC

#ifndef UNIT_TEST

#ifdef DEBUG

// If DEBUG declared, receive logs via serial port
void debug(const char *msg) {
  Serial.println(msg);
  delay(DELAY_DEBUG_MS);
}

void debug(int msg) {
  Serial.println(msg);
  delay(DELAY_DEBUG_MS);
}

#else

// If DEBUG not declared, do not generate logs
void debug(const char *msg) {}
void debug(int msg) {}

#endif

#else

// When UNIT_TEST
void debug(const char *msg) {}
void debug(int msg) {}

#endif

#endif
