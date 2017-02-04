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

#ifndef MESSAGES_ZINO_INC
#define MESSAGES_ZINO_INC

// PUMP

#define MSG_PUMP_CONFIG_AMOUNT "\2Amount? "
#define MSG_PUMP_CONFIG_ON_VALUE "\2Posit.? "
#define MSG_PUMP_CONFIG_VALUE_RANGE "\2Range? "
#define MSG_PUMP_CONFIG_SAMPLE_SHOT_TEST "\2Test? "
#define MSG_PUMP_CONFIG_SAMPLE_SHOT_TEST_YES "\2ON RUN"
#define MSG_PUMP_INFO_LAST_WATERING "Last: "
#define MSG_PUMP_INFO_WATERING_COUNT "Count: "

#define MSG_PUMP_NAME0 "PLANT_A"
#define MSG_PUMP_NAME1 "PLANT_B"
#define MSG_PUMP_NAME2 "PLANT_C"
#define MSG_PUMP_NAME3 "PLANT_D"

// LEVEL

#define MSG_LEVEL_CONFIG_MINIMUM "\2Min: "
#define MSG_LEVEL_CONFIG_MAXIMUM "\2Max: "
#define MSG_LEVEL_INFO_CURRENT_LEVEL "Lev: "
#define MSG_LEVEL_NAME "LEVEL"
#define MSG_LEVEL_CONFIG_ADVANCED "\2Advanced? "

#endif // MESSAGES_ZINO_INC
