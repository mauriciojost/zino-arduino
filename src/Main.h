#ifndef MAIN_INC
#define MAIN_INC

#ifdef INTELLIJ
#include <iom328p.h>
#endif

#include <Arduino.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <LiquidCrystal.h>
#include <UcPins.h>
#include <AppPins.h>
#include <Servo.h>
#include <Log.h>
#include <Bot.h>

void displayOnLcdString(const char *str1, const char *str2);

#endif // MAIN_INC
