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
#define BUTTON_DEBOUNCING_DELAY_MS 100
#define LEVEL_VCC_MEASURE_DELAY_MS 10

#define CLASS "Main"

volatile bool buttonModeWasPressed = false; // flag related to mode button pressed
volatile bool buttonSetWasPressed = false;  // flag related to set button pressed

volatile char nroInterruptsQueued = 0; // counter to keep track of amount of timing
                                       // interrupts queued

volatile unsigned char subCycle = 0; // counter to determine which interrupt is a cycle
                                     // and which are in the middle of a cycle

Module m;

Lcd *lcd;
Servox *servo;

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
  nroInterruptsQueued++; // increment the queue so the interrupts are treated sometime
}

/******************/
/***  CALLBACKS ***/
/******************/

void displayOnLcdString(const char *str1, const char *str2) {
  lcd->display(str1, str2);
}

int readLevel() {
  pinMode(LEVEL_VCC_PIN, OUTPUT);
  digitalWrite(LEVEL_VCC_PIN, HIGH);
  delay(LEVEL_VCC_MEASURE_DELAY_MS);
  int pinValue = digitalRead(LEVEL_ADC_PIN);
  // Refer to circuit schematic for this version.
  // If 0 is read, there is water (current circulates, transistor drives)
  // If 1 is read, there is NO water (current does not circulate, transistor open)
  int level = (pinValue == LOW ? 1: 0);

  digitalWrite(LEVEL_VCC_PIN, LOW);
  pinMode(LEVEL_VCC_PIN, INPUT);
  log(CLASS, Debug, "RDLVL:", level);
  return level;
}

void controlServo(int pos, int ms, bool pump, bool smooth) {
  digitalWrite(PUMP_PIN, pump);
  servo->controlServo(true, pos, ms, smooth);
}

/*****************/
/***** SETUP *****/
/*****************/

void setupPins() {
  pinMode(BUILTIN_LED, OUTPUT);
  pinMode(YELLOW_LED_PIN, OUTPUT);
  pinMode(LEVEL_ADC_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LCD_A, OUTPUT);

  pinMode(BUTTON_MODE_PIN, INPUT);
  pinMode(BUTTON_SET_PIN, INPUT);

  pinMode(PUMP_PIN, OUTPUT);
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
#ifdef SUBCYCLES_1
  WDTCSR = 1 << WDP1 | 1 << WDP2; // Set new watchdog timeout prescaler value (1.024 seconds)
#endif                            // SUBCYCLES_1

#ifdef SUBCYCLES_2
  WDTCSR = 1 << WDP0 | 1 << WDP2; // Set new watchdog timeout prescaler value (1.024 / 2 seconds)
#endif                            // SUBCYCLES_2

#ifdef SUBCYCLES_4
  WDTCSR = 1 << WDP2; // Set new watchdog timeout prescaler value (1.024 / 4 seconds)
#endif                // SUBCYCLES_4

#ifdef SUBCYCLES_8
  WDTCSR = 1 << WDP0 | 1 << WDP1; // Set new watchdog timeout prescaler value (1.024 / 8 seconds)
#endif                            // SUBCYCLES_8

  WDTCSR |= _BV(WDIE); // Enable the WD interrupt (note no reset)
}

void setup() {

  setupPins();
  setupLog();
  setupWDT();

  servo = new Servox(SERVO_PIN);
  m.setServoWriteFunction(controlServo);

  m.setup();
  m.setStdoutWriteFunction(displayOnLcdString);
  m.setReadLevelFunction(readLevel);
  m.setDigitalWriteFunction(digitalWrite);


  lcd = new Lcd(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);
  lcd->initialize();

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

  bool bModeStable = buttonModeWasPressed;
  bool bSetStable = buttonSetWasPressed;
  bool wdtInterrupt = nroInterruptsQueued > 0;

  if (wdtInterrupt) {
    nroInterruptsQueued--;
  }

  m.loop(bModeStable, bSetStable, wdtInterrupt);
  m.getClock()->setNroInterruptsQueued(nroInterruptsQueued);

  if (buttonModeWasPressed || buttonSetWasPressed) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(BUTTON_DEBOUNCING_DELAY_MS/4);
    digitalWrite(BUZZER_PIN, LOW);
    delay(BUTTON_DEBOUNCING_DELAY_MS*3/4);
    buttonModeWasPressed = false;
    buttonSetWasPressed = false;
  }

  if (nroInterruptsQueued <= 0) { // no interrupts queued
    nroInterruptsQueued = 0;
    enterSleep();
  }
}

#endif // UNIT_TEST
