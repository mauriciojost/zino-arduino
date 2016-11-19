#include <Log.h>

const char *logLevelStr[4] = {"DEBUG", "INFO", "WARN", "ERROR"};

#ifndef UNIT_TEST

// !UNIT_TEST, SO ON-BOARD EXECUTION
#ifdef DEBUG

// Receive logs via serial port

void setupLog() {
  Serial.begin(SERIAL_BAUDS);
}

void log(const char *clz, LogLevel l, const char *msg) {
  if (LOG_LEVEL <= l) {
    Serial.print("[");
    Serial.print(clz);
    Serial.print("] ");
    Serial.print(logLevelStr[l]);
    Serial.print(": ");
    Serial.println(msg);
    delay(DELAY_DEBUG_MS);
  }
}

void log(const char *clz, LogLevel l, int msg) {
  if (LOG_LEVEL <= l) {
    Serial.print("[");
    Serial.print(clz);
    Serial.print("] ");
    Serial.print(logLevelStr[l]);
    Serial.print(": ");
    Serial.println(msg);
    delay(DELAY_DEBUG_MS);
  }
}

void log(const char *clz, LogLevel l, const char *msg, int i) {
  if (LOG_LEVEL <= l) {
    Serial.print("[");
    Serial.print(clz);
    Serial.print("] ");
    Serial.print(logLevelStr[l]);
    Serial.print(": ");
    Serial.print(msg);
    Serial.print(" ");
    Serial.println(i);
    delay(DELAY_DEBUG_MS);
  }
}

void log(const char *clz, LogLevel l, const char *msg1, const char *msg2) {
  if (LOG_LEVEL <= l) {
    Serial.print("[");
    Serial.print(clz);
    Serial.print("] ");
    Serial.print(logLevelStr[l]);
    Serial.print(": ");
    Serial.print(msg1);
    Serial.print(" ");
    Serial.println(msg2);
    delay(DELAY_DEBUG_MS);
  }
}

#else // !DEBUG

// Do not generate logs
void setupLog() {}
void log(const char *clz, LogLevel l, const char *msg) {}
void log(const char *clz, LogLevel l, int msg) {}
void log(const char *clz, LogLevel l, const char *msg, int i) {}
void log(const char *clz, LogLevel l, const char *msg1, const char *msg2) {}

#endif // DEBUG

#else // UNIT_TEST, SO ON-PC EXECUTION

void setupLog() {}

void log(const char *clz, LogLevel l, const char *msg) {
  if (LOG_LEVEL <= l) {
    printf("[%8.8s] [%5.5s]: %s\n", clz, logLevelStr[l], msg);
  }
}
void log(const char *clz, LogLevel l, int msg) {
  if (LOG_LEVEL <= l) {
    printf("[%8.8s] [%5.5s]: %d\n", clz, logLevelStr[l], msg);
  }
}

void log(const char *clz, LogLevel l, const char *msg, int i) {
  if (LOG_LEVEL <= l) {
    printf("[%8.8s] [%5.5s]: %s %d\n", clz, logLevelStr[l], msg, i);
  }
}

void log(const char *clz, LogLevel l, const char *msg1, const char *msg2) {
  if (LOG_LEVEL <= l) {
    printf("[%8.8s] [%5.5s]: %s %s\n", clz, logLevelStr[l], msg1, msg2);
  }
}

void delay(int ms) {}

#endif // UNIT_TEST
