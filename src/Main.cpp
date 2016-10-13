#ifndef UNIT_TEST

#include <Main.h>

#define SERIAL_BAUDS 115200
#define BUTTON_DEBOUNCING_DELAY_MS 150
#define SERVO_CONTROL_CYCLES 200

volatile bool wdtWasTriggered = true;
volatile bool button0WasPressed = false;
volatile bool button1WasPressed = false;

Servo servo;
Bot bot(displayOnLcdString);
LiquidCrystal lcd(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_D4_PIN, LCD_D5_PIN,
                  LCD_D6_PIN, LCD_D7_PIN);

/*****************/
/****** ISR ******/
/*****************/

void ISR_Button0() { button0WasPressed = true; }

void ISR_Button1() { button1WasPressed = true; }

ISR(WDT_vect) {
  if (!wdtWasTriggered) {
    wdtWasTriggered = true;
  } else {
    log(Warn, "WDTO");
  }
}

/*****************/
/***** SETUP *****/
/*****************/

void setupPins() {
  pinMode(BUILTIN_LED, OUTPUT);

  pinMode(BUTTON0, INPUT);
  pinMode(BUTTON1, INPUT);

  pinMode(SERVO_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(BUTTON0), ISR_Button0, RISING);
  attachInterrupt(digitalPinToInterrupt(BUTTON1), ISR_Button1, RISING);
}

void setupWDT() {
  // Set the WDT so that there is an interrupt every ~8 seconds.
  MCUSR &= ~(1 << WDRF);              // Clear the reset flag
  WDTCSR |= (1 << WDCE) | (1 << WDE); // In order to change WDE or the
                                      // prescaler, set WDCE (this will allow
                                      // updates for 4 clock cycles)
#ifdef FAST
  WDTCSR =
      1 << WDP0 |
      1 << WDP2; // Set new watchdog timeout prescaler value (faster if BEBUG)
#else

#ifdef CYCLE_OF_1S
  WDTCSR = 1 << WDP1 | 1 << WDP2; // Set new watchdog timeout prescaler value (1.024 seconds)
#else
  WDTCSR = 1 << WDP0 | 1 << WDP3; // Set new watchdog timeout prescaler value (8.192 seconds)
#endif // CYCLE_OF_8S

#endif // FAST
  WDTCSR |= _BV(WDIE); // Enable the WD interrupt (note no reset)
}

void setup() {
  lcd.begin(16, 2);
  Serial.begin(SERIAL_BAUDS);
  setupPins();
  setupWDT();
}

/*****************/
/*****  LCD  *****/
/*****************/

void displayOnLcdString(const char *str1, const char *str2) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(str1);
  lcd.setCursor(0, 1);
  lcd.print(str2);
}


/*****************/
/***** MAIN  *****/
/*****************/

void enterSleep(void) {
  log(Info, "SLEEP");
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Could use SLEEP_MODE_PWR_SAVE, or
                                       // SLEEP_MODE_PWR_DOWN for
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
  delay(10);
}

void servoControl(bool drive, int servoPosition) {
  if (drive) {
    servo.attach(SERVO_PIN);
    for (int i=0; i<SERVO_CONTROL_CYCLES; i++) {
      servo.write(servoPosition);
      stroboscope();
    }
  } else {
    servo.detach();
  }
}

void pumpControl(bool drive) {
  if (drive) {
    digitalWrite(PUMP_PIN, HIGH);
  } else {
    digitalWrite(PUMP_PIN, LOW);
  }
}

void loop() {

  stroboscope();

  log(Info, "\n\n\nLOOP");

  bot.cycle(button0WasPressed, button1WasPressed, wdtWasTriggered);

  servoControl(bot.isServoDriven(), bot.barrel.servoPosition);
  pumpControl(bot.isPumpDriven());

  if (button0WasPressed || button1WasPressed) {
    delay(BUTTON_DEBOUNCING_DELAY_MS);
    button0WasPressed = false;
    button1WasPressed = false;
  }
  if (wdtWasTriggered) {
    wdtWasTriggered = false;
  }

  enterSleep();
}

#endif
