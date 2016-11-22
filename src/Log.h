#ifndef LOG_INC
#define LOG_INC

#include <Misc.h>

#define DELAY_DEBUG_MS 5
#define SERIAL_BAUDS 115200

// ON-PC SHELL COLORS (to be used with printf in PC tests)
// http://ascii-table.com/ansi-escape-sequences.php
#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KMAG "\x1B[35m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KWHTBLU "\x1B[37;44m"

enum LogLevel { Debug = 0, Info = 1, Warn = 2, Error = 3 };

void setupLog();
void log(const char *clz, LogLevel l, const char *msg);
void log(const char *clz, LogLevel l, int msg);
void log(const char *clz, LogLevel l, const char *msg, int i);
void log(const char *clz, LogLevel l, const char *msg1, const char *msg2);

#ifndef UNIT_TEST

#include <Arduino.h>

#else // UNIT_TEST, SO ON-PC EXECUTION

#include <stdio.h>
void delay(int ms);

#endif // UNIT_TEST

#endif // LOG_INC
