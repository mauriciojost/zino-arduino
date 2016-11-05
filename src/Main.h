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
#include <Log.h>
#include <Bot.h>
#include <Pump.h>
#include <Level.h>
#include <Actor.h>

void displayOnLcdString(const char *str1, const char *str2);
int readLevel();

#endif // MAIN_INC
