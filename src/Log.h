#ifndef LOG_INC
#define LOG_INC

#define DELAY_DEBUG_MS 10

enum LogLevel { Debug = 0, Info = 1, Warn = 2, Error = 3 };

void log(LogLevel l, const char *msg);
void log(LogLevel l, int msg);
void log(LogLevel l, const char *msg, int i);

#ifndef UNIT_TEST

#include <Arduino.h>

#else // UNIT_TEST, SO ON-PC EXECUTION

#include <stdio.h>
void delay(int ms);

#endif // UNIT_TEST

#endif // LOG_INC
