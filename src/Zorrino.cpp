#include <Arduino.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <avr/sleep.h>
#include <LiquidCrystal.h>
#include <Zorrino.h>
#include <Servo.h>
#include <Debug.h>
#include <UI.h>

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
#define LCD_RW GROUND
#define LCD_VSS GROUND
#define LCD_VCC VCC
#define LCD_V0 GROUND

volatile bool wdt_was_triggered = true;
volatile bool button0_was_triggered = false;
volatile bool button1_was_triggered = false;
LiquidCrystal lcd(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);
Servo servo;
UI ui;

/*****************/
/****** ISR ******/
/*****************/

void ISR_Button0() {
    button0_was_triggered = true;
}

void ISR_Button1() {
    button1_was_triggered = true;
}

ISR(WDT_vect) {
    if (!wdt_was_triggered) {
        wdt_was_triggered = true;
    } else {
        debug("WDT overrun!!!");
    }
}


/*****************/
/***** SETUP *****/
/*****************/

void setupPins() {
    pinMode(BUILTIN_LED, OUTPUT);

    pinMode(BUTTON0, INPUT);
    pinMode(BUTTON1, INPUT);

    attachInterrupt(digitalPinToInterrupt(BUTTON0), ISR_Button0, RISING);
    attachInterrupt(digitalPinToInterrupt(BUTTON1), ISR_Button1, RISING);
}

void setupWDT() {
    MCUSR &= ~(1<<WDRF); // Clear the reset flag
    WDTCSR |= (1<<WDCE) | (1<<WDE); // In order to change WDE or the prescaler, set WDCE (this will allow updates for 4 clock cycles)
    WDTCSR = 1<<WDP0 | 1<<WDP3; // Set new watchdog timeout prescaler value
    WDTCSR |= _BV(WDIE); // Enable the WD interrupt (note no reset)
}

void setup() {
    Serial.begin(9600);
    servo.attach(SERVO);
    setupPins();
    setupWDT();
}


/*****************/
/***** MAIN  *****/
/*****************/

void enterSleep(void) {
    set_sleep_mode(SLEEP_MODE_PWR_SAVE); // Could also use SLEEP_MODE_PWR_DOWN for lowest power consumption
    sleep_enable();
    sleep_mode(); // Now enter sleep mode

    /* The program will continue from here after the WDT timeout */

    sleep_disable(); // First thing to do is disable sleep
    power_all_enable(); // Re-enable the peripherals
}


void loop() {
    debug("Awake!");
    if (button0_was_triggered || button1_was_triggered) {
        ui.run(button0_was_triggered, button1_was_triggered, false);
        debug("Button interrupt");
        button0_was_triggered = false;
        button1_was_triggered = false;
    } else if (wdt_was_triggered) {
        ui.run(false, false, wdt_was_triggered);
        debug("Timer interrupt");
        wdt_was_triggered = false;
    }
    enterSleep(); // Re-enter sleep mode
}

