/*
 * ZINO: low-budget autonomous plant care system.
 *
 * Copyright 2016 Mauricio Jost
 * Contact: mauriciojostx@gmail.com
 *
 * This file is part of ZINO.
 *
 * ZINO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * ZINO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with ZINO. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MAIN_INC
#define MAIN_INC

#ifdef INTELLIJ
#include <iom328p.h>
#endif

#include <Arduino.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <hardware/UcPins.h>
#include <hardware/AppPins.h>
#include <ui/Lcd.h>
#include <Log.h>
#include <Bot.h>
#include <Clock.h>
#include <Module.h>
#include <actors/Actor.h>
#include <actors/Pump.h>
#include <actors/Level.h>
#include <actors/Delayer.h>
#include <EEPROM.h>

#endif // MAIN_INC
