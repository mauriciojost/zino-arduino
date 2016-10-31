#include <Log.h>

#ifndef UNIT_TEST

// !UNIT_TEST, SO ON-BOARD EXECUTION
#ifdef DEBUG

// Receive logs via serial port

const char *logLevelStr[4] = {"DEBUG", "INFO", "WARN", "ERROR"};

void setupLog() { Serial.begin(SERIAL_BAUDS); }

void log(LogLevel l, const char *msg) {
  if (LOG_LEVEL <= l) {
    Serial.print(logLevelStr[l]);
    Serial.print(": ");
    Serial.println(msg);
    delay(DELAY_DEBUG_MS);
  }
}

void log(LogLevel l, int msg) {
  if (LOG_LEVEL <= l) {
    Serial.print(logLevelStr[l]);
    Serial.print(": ");
    Serial.println(msg);
    delay(DELAY_DEBUG_MS);
  }
}

void log(LogLevel l, const char *msg, int i) {
  if (LOG_LEVEL <= l) {
    Serial.print(logLevelStr[l]);
    Serial.print(": ");
    Serial.print(msg);
    Serial.println(i);
    delay(DELAY_DEBUG_MS);
  }
}

#else // !DEBUG

// Do not generate logs
void setupLog() {}
void log(LogLevel l, const char *msg) {}
void log(LogLevel l, int msg) {}
void log(LogLevel l, const char *msg, int i) {}

#endif // DEBUG

#else // UNIT_TEST, SO ON-PC EXECUTION

void setupLog() {}

void log(LogLevel l, const char *msg) {
  if (LOG_LEVEL <= l) {
    switch (l) {
    case Debug:
      printf("DEBUG: %s\n", msg);
      break;
    case Info:
      printf("INFO: %s\n", msg);
      break;
    case Warn:
      printf("WARN: %s\n", msg);
      break;
    case Error:
      printf("ERROR: %s\n", msg);
      break;
    }
  }
}
void log(LogLevel l, int msg) {
  if (LOG_LEVEL <= l) {
    switch (l) {
    case Debug:
      printf("DEBUG: %d\n", msg);
      break;
    case Info:
      printf("INFO: %d\n", msg);
      break;
    case Warn:
      printf("WARN: %d\n", msg);
      break;
    case Error:
      printf("ERROR: %d\n", msg);
      break;
    }
  }
}
void log(LogLevel l, const char *msg, int i) {
  if (LOG_LEVEL <= l) {
    switch (l) {
    case Debug:
      printf("DEBUG: %s %d\n", msg, i);
      break;
    case Info:
      printf("INFO: %s %d\n", msg, i);
      break;
    case Warn:
      printf("WARN: %s %d\n", msg, i);
      break;
    case Error:
      printf("ERROR: %s %d\n", msg, i);
      break;
    }
  }
}

void delay(int ms) {}

#endif // UNIT_TEST
