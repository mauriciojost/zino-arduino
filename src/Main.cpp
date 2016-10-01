#ifndef UNIT_TEST

#include <Main.h>

#define SERIAL_BAUDS 9600

volatile bool wdt_was_triggered = true;
volatile bool button0_was_triggered = false;
volatile bool button1_was_triggered = false;
LiquidCrystal lcd(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_D4_PIN, LCD_D5_PIN,
                  LCD_D6_PIN, LCD_D7_PIN);
Servo servo;

void displayOnLcd(const char* str){lcd.setCursor(0, 0); lcd.print(str); lcd.setCursor(0, 1); lcd.print(str + 16 + 1);}

Bot bot(displayOnLcd);

/*****************/
/****** ISR ******/
/*****************/

void ISR_Button0() { button0_was_triggered = true; }

void ISR_Button1() { button1_was_triggered = true; }

ISR(WDT_vect) {
  if (!wdt_was_triggered) {
    wdt_was_triggered = true;
  } else {
    debug("WDTO");
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
  MCUSR &= ~(1 << WDRF);              // Clear the reset flag
  WDTCSR |= (1 << WDCE) | (1 << WDE); // In order to change WDE or the
                                      // prescaler, set WDCE (this will allow
                                      // updates for 4 clock cycles)
  WDTCSR = 1 << WDP0 | 1 << WDP3; // Set new watchdog timeout prescaler value
  WDTCSR |= _BV(WDIE);            // Enable the WD interrupt (note no reset)
}

void setup() {
  Serial.begin(SERIAL_BAUDS);
  servo.attach(SERVO_PIN);
  setupPins();
  setupWDT();
}

/*****************/
/***** MAIN  *****/
/*****************/

void enterSleep(void) {
  debug("SLEEP");
  set_sleep_mode(SLEEP_MODE_PWR_SAVE); // Could also use SLEEP_MODE_PWR_DOWN for
                                       // lowest power consumption
  sleep_enable();
  sleep_mode();

  /* The program will continue from here after the WDT timeout */

  sleep_disable();    // First thing to do is disable sleep
  power_all_enable(); // Re-enable the peripherals
}

void loop() {
  debug("AWAKE");
  if (button0_was_triggered || button1_was_triggered) {
    bot.run(button0_was_triggered, button1_was_triggered, false);
    debug("BUTTON");
    button0_was_triggered = false;
    button1_was_triggered = false;
  } else if (wdt_was_triggered) {
    bot.run(false, false, wdt_was_triggered);
    debug("TIMER");
    wdt_was_triggered = false;
  }
  enterSleep(); // Re-enter sleep mode
}

#endif
