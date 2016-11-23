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

#ifndef UC_PINS_INC
#define UC_PINS_INC

#define UC_PIN_1 1
#define UC_PIN_2 2
#define UC_PIN_3 3
#define UC_PIN_4 4
#define UC_PIN_5 5
#define UC_PIN_6 6
#define UC_PIN_7 7
#define UC_PIN_8 8
#define UC_PIN_9 9
#define UC_PIN_10 10
#define UC_PIN_11 11
#define UC_PIN_12 12
#define UC_PIN_13 13

#ifndef UNIT_TEST // On arduino

#define UC_PIN_A0 A0
#define UC_PIN_A1 A1
#define UC_PIN_A2 A2
#define UC_PIN_A3 A3
#define UC_PIN_A4 A4
#define UC_PIN_A5 A5
#define UC_PIN_A6 A6
#define UC_PIN_A7 A7

#else // UNIT_TEST, on PC

#define UC_PIN_A0 0
#define UC_PIN_A1 1
#define UC_PIN_A2 2
#define UC_PIN_A3 3
#define UC_PIN_A4 4
#define UC_PIN_A5 5
#define UC_PIN_A6 6
#define UC_PIN_A7 7

#define LOW 0
#define HIGH 1

#endif // UNIT_TEST

#endif // UC_PINS_INC
