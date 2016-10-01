#ifndef UNIT_TEST

#include <Main.h>

#define SERIAL_BAUDS 115200

volatile bool wdtWasTriggered = true;
volatile bool button0WasPressed = false;
volatile bool button1WasPressed = false;
LiquidCrystal lcd(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_D4_PIN, LCD_D5_PIN,
                  LCD_D6_PIN, LCD_D7_PIN);
Servo servo;

void displayOnLcdString(const char *str1, const char *str2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(str1);
  lcd.setCursor(0, 1);
  lcd.print(str2);
}

Bot bot(displayOnLcdString);

/*****************/
/****** ISR ******/
/*****************/

void ISR_Button0() { button0WasPressed = true; }

void ISR_Button1() { button1WasPressed = true; }

ISR(WDT_vect) {
  if (!wdtWasTriggered) {
    wdtWasTriggered = true;
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
  // Set the WDT so that there is an interrupt every ~8 seconds.
  MCUSR &= ~(1 << WDRF);              // Clear the reset flag
  WDTCSR |= (1 << WDCE) | (1 << WDE); // In order to change WDE or the
                                      // prescaler, set WDCE (this will allow
                                      // updates for 4 clock cycles)
  WDTCSR = 1 << WDP0 | 1 << WDP3; // Set new watchdog timeout prescaler value
  WDTCSR |= _BV(WDIE);            // Enable the WD interrupt (note no reset)
}

void setup() {
  lcd.begin(16, 2);
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

void stroboscope() {
  digitalWrite(BUILTIN_LED, HIGH);
  delay(10);
  digitalWrite(BUILTIN_LED, LOW);
}

void loop() {
  debug("AWAKE");
  if (button0WasPressed || button1WasPressed) {
    debug("BUTTON");
    bot.run(button0WasPressed, button1WasPressed, false);
    button0WasPressed = false;
    button1WasPressed = false;
  } else if (wdtWasTriggered) {
    debug("TIMER");
    bot.run(false, false, wdtWasTriggered);
    wdtWasTriggered = false;
  }
  stroboscope();
  enterSleep();
}

#endif
