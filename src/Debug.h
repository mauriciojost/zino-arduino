#define DELAY_DEBUG_MS 100
#ifndef UNIT_TEST


#ifdef DEBUG

// If DEBUG declared, receive logs via serial port
void debug(char* msg) {
  Serial.println(msg);
  delay(DELAY_DEBUG_MS);
}

#else

// If DEBUG not declared, do not generate logs
void debug(char* msg) {}

#endif

#else

// When UNIT_TEST
void debug(char* msg) { }

#endif
