/*
 * The LCD circuit:
 *  LCD RS pin to digital pin 12
 *  LCD Enable pin to digital pin 11
 *  LCD D4 pin to digital pin 7
 *  LCD D5 pin to digital pin 6
 *  LCD D6 pin to digital pin 5
 *  LCD D7 pin to digital pin 4
 *  LCD R/W pin to ground
 *  LCD VSS pin to ground
 *  LCD VCC pin to 5V
 *  LCD v0 pin to ground
 *
 * The buttons circuit:
 *  LEFT BUTTON to A0
 *  RIGHT BUTTON to A1
 *
 * The servos circuit:
 *  SERVO 1 to A2
 *  SERVO 2 to A3
 *
 * HOME/.platformio/packages/framework-arduinoavr/cores/arduino/

 */

#include <LiquidCrystal.h>
#include <Arduino.h>
#include <Servo.h>
#include <avr/sleep.h>
#include <Zorrino.h>
#include <UI.h>
//#include <TimerOne.h>
#include <avr/power.h>
#include <avr/wdt.h>

#define DELAY 100
#define BUTTON0 UC_PIN_2
#define BUTTON1 UC_PIN_3
#define BUILTIN_LED UC_PIN_13
#define SERVO UC_PIN_A2

#define LCD_RS_PIN UC_PIN_12
#define LCD_ENABLE_PIN UC_PIN_11
#define LCD_D4_PIN UC_PIN_7
#define LCD_D5_PIN UC_PIN_6
#define LCD_D6_PIN UC_PIN_5
#define LCD_D7_PIN UC_PIN_4


volatile bool wdt_was_triggered = true;
volatile bool button0_was_triggered = false;
volatile bool button1_was_triggered = false;
LiquidCrystal lcd(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);
Servo servo;
UI ui;

/*
size_t debug(const __FlashStringHelper * msg) {
    Serial.println(msg);
    delay(100);
}
*/

size_t debug(const char * msg) {
    Serial.println(msg);
    delay(100);
}

void wakeUpButton0() {
    if (!button0_was_triggered) {
        button0_was_triggered = true;
    } else {
        debug("button 0 overpressed!!!");
    }
}

void wakeUpButton1() {
    if (!button1_was_triggered) {
        button1_was_triggered = true;
    } else {
        debug("button 1 overpressed!!!");
    }
}

ISR(WDT_vect) {
  if (!wdt_was_triggered) {
    wdt_was_triggered = true;
  } else {
    debug("WDT Overrun!!!");
  }
}


void enterSleep(void) {
  set_sleep_mode(SLEEP_MODE_PWR_SAVE); // Could also use SLEEP_MODE_PWR_DOWN for lowest power consumption
  sleep_enable();
  sleep_mode(); // Now enter sleep mode

  /* The program will continue from here after the WDT timeout */

  sleep_disable(); // First thing to do is disable sleep
  power_all_enable(); // Re-enable the peripherals
}

void setupWDT() {
    MCUSR &= ~(1<<WDRF); // Clear the reset flag
    WDTCSR |= (1<<WDCE) | (1<<WDE); // In order to change WDE or the prescaler, set WDCE (this will allow updates for 4 clock cycles)
    WDTCSR = 1<<WDP0 | 1<<WDP3; // Set new watchdog timeout prescaler value (8.0 seconds)
    WDTCSR |= _BV(WDIE); // Enable the WD interrupt (note no reset)
}

void setup() {

  Serial.begin(9600);
  debug("Initialising...");

  pinMode(BUTTON0, INPUT);
  pinMode(BUTTON1, INPUT);
  pinMode(BUILTIN_LED, OUTPUT);

  servo.attach(SERVO);

  attachInterrupt(digitalPinToInterrupt(BUTTON0), wakeUpButton0, RISING);
  attachInterrupt(digitalPinToInterrupt(BUTTON1), wakeUpButton1, RISING);

  setupWDT();

  debug("Initialisation complete.");

}


void loop() {
  if (wdt_was_triggered) {
    debug("Timer interrupt");
    wdt_was_triggered = false;
    enterSleep(); // Re-enter sleep mode
  } else if (button0_was_triggered) {
    debug("Button 0 interrupt");
    button0_was_triggered = false;
    enterSleep(); // Re-enter sleep mode
  } else if (button1_was_triggered) {
    debug("Button 1 interrupt");
    button0_was_triggered = false;
    enterSleep(); // Re-enter sleep mode
  } else {
    /* Do nothing. */
  }
}
