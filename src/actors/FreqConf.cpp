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

#include <actors/FreqConf.h>

const char *frequencyDescriptions[DelimiterAmountOfFrequencies] =
    {"1/month", "2/month", "1/week", "2/week", "3/week", "1/day", "2/day", "1/hour", "2/hour", "1/5min", "1/2min"};

FreqConf::FreqConf(Frequency f) {
  freq = f;
  matchInvalidateCounter = 0;
}

void FreqConf::setFrequency(Frequency f) {
  freq = f;
}

Frequency FreqConf::getFrequency() {
  return freq;
}

void FreqConf::setInvalidateCounter(int i) {
  matchInvalidateCounter = i;
}

int FreqConf::getInvalidateCounter() {
  return matchInvalidateCounter;
}

void FreqConf::setNextFrequency() {
  freq = (Frequency)((freq + 1) % DelimiterAmountOfFrequencies);
}

const char *FreqConf::getFrequencyDescription() {
  return frequencyDescriptions[freq];
}

