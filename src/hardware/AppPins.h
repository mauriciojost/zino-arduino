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

#ifndef APP_PINS_INC
#define APP_PINS_INC

#include <hardware/UcPins.h>

// http://www.dominicdube.com/blog/arduino-pro-mini-pinout/

// ONBOARD
#define BUILTIN_LED UC_PIN_13

// BUTTONS
#define BUTTON_MODE_PIN UC_PIN_2
#define BUTTON_SET_PIN UC_PIN_3

// PUMP
#define PUMP_PIN UC_PIN_10

// SERVO
#define SERVO_PIN UC_PIN_9

// LEVEL
#define LEVEL_VCC_PIN UC_PIN_A3
#define LEVEL_ADC_PIN UC_PIN_A1
#define LEVEL_BUZZER_PIN UC_PIN_A2

// LCD
#define LCD_VSS GROUND
#define LCD_VCC VCC
#define LCD_V0 GROUND
#define LCD_RS_PIN UC_PIN_12
#define LCD_RW GROUND
#define LCD_ENABLE_PIN UC_PIN_11
#define LCD_D0_PIN GROUND
#define LCD_D1_PIN GROUND
#define LCD_D2_PIN GROUND
#define LCD_D3_PIN GROUND
#define LCD_D4_PIN UC_PIN_4
#define LCD_D5_PIN UC_PIN_5
#define LCD_D6_PIN UC_PIN_6
#define LCD_D7_PIN UC_PIN_7
#define LCD_A UC_PIN_A0 // via RES of 1K
#define LCD_K GROUND

#endif // APP_PINS_INC
