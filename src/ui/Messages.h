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

#ifndef MESSAGES_INC
#define MESSAGES_INC

// BOT

#define MSG_BOT_STATE_WELCOME "\1INTRO\1"
#define MSG_BOT_STATE_WELCOME_ZINO "ZINO " STRINGIFY(VERSION)

#define MSG_BOT_STATE_HELP "HELP"
#define MSG_BOT_STATE_HELP_UP "\1 CHANGES MODE"
#define MSG_BOT_STATE_HELP_DOWN "\2 SETS VALUE"

#define MSG_BOT_STATE_RUN "\3RUN\3 "

#define MSG_BOT_STATE_CONFIGURABLES "SET"
#define MSG_BOT_DONE_CONFIGURING_CONFIGURABLES "\1PRESS\1"

#define MSG_BOT_STATE_FREQUENCIES "FREQ"
#define MSG_BOT_DONE_CONFIGURING_FREQUENCIES MSG_BOT_DONE_CONFIGURING_CONFIGURABLES
#define MSG_BOT_FREQUENCY_SET "\2Freq? "
#define MSG_BOT_FREQUENCY_INFO "Freq: "

// PUMP

#define MSG_PUMP_CONFIG_AMOUNT "\2Amount? "
#define MSG_PUMP_CONFIG_SAMPLE_SHOT_DONOT_TEST "\2Test?"
#define MSG_PUMP_CONFIG_SAMPLE_SHOT_DO_TEST "Yes"
#define MSG_PUMP_INFO_LAST_WATERING "Last: "

#define MSG_PUMP_NAME0 "PLANT_A"
#define MSG_PUMP_NAME1 "PLANT_B"
#define MSG_PUMP_NAME2 "PLANT_C"
#define MSG_PUMP_NAME3 "PLANT_D"

// LEVEL

#define MSG_LEVEL_CONFIG_MINIMUM "\2Min: "
#define MSG_LEVEL_INFO_CURRENT_LEVEL "Lev: "
#define MSG_LEVEL_NAME "LEVEL"

// CLOCK

#define MSG_CLOCK_NAME "CLOCK"
#define MSG_CLOCK_CONFIG_HOUR "\2H? "
#define MSG_CLOCK_CONFIG_MINUTE "\2M? "
#define MSG_CLOCK_CONFIG_SECOND "\2S? "
#define MSG_CLOCK_CONFIG_FACTOR_UP "\2F+? "
#define MSG_CLOCK_CONFIG_FACTOR_DOWN "\2F-? "

#endif // MESSAGES_INC
