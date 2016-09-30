#ifndef UNIT_TEST


#ifdef DEBUG

// If DEBUG declared, receive logs via serial port

#define DELAY_DEBUG_MS 100

void debug(String msg) {
  Serial.println(msg);
  delay(DELAY_DEBUG_MS);
}

void debug(int msg) {
  Serial.println(msg);
  delay(DELAY_DEBUG_MS);
}

#else

// If DEBUG not declared, do not generate logs

void debug(String msg) {}

void debug(int msg) {}

#endif

#else

// When UNIT_TEST

String lastStringMessage;
int lastIntMessage;

void debug(String msg) {
  lastStringMessage = msg;
}

void debug(int msg) {
  lastIntMessage = msg;
}

#endif
