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

#ifndef UNIT_TEST

#include <Main.h>
#ifdef DEBUG
#define BUTTON_DEBOUNCING_DELAY_MS 50
#else
#define BUTTON_DEBOUNCING_DELAY_MS 100
#endif // DEBUG
#define LEVEL_VCC_MEASURE_DELAY_MS 10
#define FACTOR_EEPROM_ADDRESS 0
#define CLASS "Main"

volatile bool wdtWasTriggered = true;       // flag related to periodic WDT interrupts
volatile bool buttonModeWasPressed = false; // flag related to mode button pressed
volatile bool buttonSetWasPressed = false;  // flag related to set button pressed

volatile bool overruns = 0; // counter to keep track of amount of timing
                            // interrupts lost because of overrun

Module m;

/*****************/
/** INTERRUPTS ***/
/*****************/

void ISR_ButtonMode() {
  buttonModeWasPressed = true;
}

void ISR_ButtonSet() {
  buttonSetWasPressed = true;
}

ISR(WDT_vect) {
  if (!wdtWasTriggered) {
    wdtWasTriggered = true;
  } else {
    overruns++;
  }
}

/******************/
/***  CALLBACKS ***/
/******************/

void displayOnLcdString(const char *str1, const char *str2) {
  m.getLcd()->display(str1, str2);
}

void saveFactor(bool setPressed) {
  if (setPressed) {
    float eepromFactor;
    EEPROM.get(FACTOR_EEPROM_ADDRESS, eepromFactor);
    float clockFactor = m.getClock()->getFactor();
    if (clockFactor != eepromFactor) {
      log(CLASS, Debug, "F (PUT) : ", (int)(clockFactor*10000));
      EEPROM.put(FACTOR_EEPROM_ADDRESS, clockFactor);
    }
  }
}

int readLevel() {
  pinMode(LEVEL_VCC_PIN, OUTPUT);
  digitalWrite(LEVEL_VCC_PIN, HIGH);
  delay(LEVEL_VCC_MEASURE_DELAY_MS);
  int level = digitalRead(LEVEL_ADC_PIN);
  digitalWrite(LEVEL_VCC_PIN, LOW);
  pinMode(LEVEL_VCC_PIN, INPUT);
  log(CLASS, Debug, "RDLVL:", level);
  return level;
}


/*****************/
/***** SETUP *****/
/*****************/

void setupPins() {
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(LEVEL_ADC_PIN, INPUT);
  pinMode(LEVEL_BUZZER_PIN, OUTPUT);
  pinMode(LCD_A, OUTPUT);

  pinMode(BUTTON_MODE_PIN, INPUT);
  pinMode(BUTTON_SET_PIN, INPUT);

  pinMode(PUMP0_PIN, OUTPUT);
  pinMode(PUMP1_PIN, OUTPUT); // same
  pinMode(SERVO_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(BUTTON_MODE_PIN), ISR_ButtonMode, RISING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_SET_PIN), ISR_ButtonSet, RISING);
}

void setupWDT() {
  // Set the WDT so that there is an interrupt every ~8 seconds.
  MCUSR &= ~(1 << WDRF);              // Clear the reset flag
  WDTCSR |= (1 << WDCE) | (1 << WDE); // In order to change WDE or the
                                      // prescaler, set WDCE (this will allow
                                      // updates for 4 clock cycles)
#ifdef FAST
  WDTCSR = 1 << WDP0 | 1 << WDP2; // Set new watchdog timeout prescaler value (faster if BEBUG)
#else

#ifdef CYCLE_OF_1S
  WDTCSR = 1 << WDP1 | 1 << WDP2; // Set new watchdog timeout prescaler value (1.024 seconds)
#endif // CYCLE_OF_1S
#ifdef CYCLE_OF_8S
  WDTCSR = 1 << WDP0 | 1 << WDP3; // Set new watchdog timeout prescaler value (8.192 seconds)
#endif // CYCLE_OF_8S

#endif                 // FAST
  WDTCSR |= _BV(WDIE); // Enable the WD interrupt (note no reset)
}

float setupFactor() {
  float factor = 0.0f; // initial value will be dropped
  EEPROM.get(FACTOR_EEPROM_ADDRESS, factor);
  log(CLASS, Debug, "F (READ) : ", (int)(factor * 10000));
  return factor;
}

void setup() {
  setupPins();
  setupLog();
  setupWDT();
  m.setup();
  m.setFactor(setupFactor());
  m.setStdoutWriteFunction(displayOnLcdString);
  m.setReadLevelFunction(readLevel);
  m.setDigitalWriteFunction(digitalWrite);
}

/*****************/
/***** MAIN  *****/
/*****************/

void enterSleep(void) {
  log(CLASS, Info, "SLEEP");
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // SLEEP_MODE_PWR_SAVE, SLEEP_MODE_STANDBY, SLEEP_MODE_IDLE
  digitalWrite(BUILTIN_LED, LOW);
  sleep_enable();
  sleep_mode();
  /*** THE PROGRAM WILL CONTINUE FROM HERE AFTER THE WDT TIMEOUT ***/
  digitalWrite(BUILTIN_LED, HIGH);
  sleep_disable();    // First thing to do is disable sleep
  power_all_enable(); // Re-enable the peripherals
}

void loop() {

  bool localWdt = wdtWasTriggered;

  if (wdtWasTriggered) {
    wdtWasTriggered = false;
  }

  log(CLASS, Debug, "OVRN: ", overruns);
  bool bModeStable = buttonModeWasPressed && digitalRead(BUTTON_MODE_PIN);
  bool bSetStable = buttonSetWasPressed && digitalRead(BUTTON_SET_PIN);

  m.loop(bModeStable, bSetStable, localWdt);

  saveFactor(buttonSetWasPressed);

  if (buttonModeWasPressed || buttonSetWasPressed) {
    delay(BUTTON_DEBOUNCING_DELAY_MS);
    buttonModeWasPressed = false;
    // disable set button only if no longer being pressed
    if (!digitalRead(BUTTON_SET_PIN)) {
      buttonSetWasPressed = false;
    }
  }

  enterSleep();
}

#endif // UNIT_TEST
