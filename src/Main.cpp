#ifndef UNIT_TEST

#include <Main.h>

#ifdef DEBUG
#define BUTTON_DEBOUNCING_DELAY_MS 120
#else
#define BUTTON_DEBOUNCING_DELAY_MS 320
#endif // DEBUG
#define PUMP_ACTIVATION_OFFSET_UNIT 60

volatile bool wdtWasTriggered = true;       // flag related to periodic WDT interrupts
volatile bool buttonModeWasPressed = false; // flag related to mode button pressed
volatile bool buttonSetWasPressed = false;  // flag related to set button pressed

const int amountOfActors = 3;
Pump pump0(MSG_PUMP_NAME0, PUMP_ACTIVATION_OFFSET_UNIT * 0);
Pump pump1(MSG_PUMP_NAME1, PUMP_ACTIVATION_OFFSET_UNIT * 1);
Level level(MSG_LEVEL_NAME, readLevel);
Actor *actors[amountOfActors] = {&pump0, &pump1, &level};

Bot bot(displayOnLcdString, actors, amountOfActors);
LiquidCrystal lcd(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

byte modeButtonIcon[8] = {
    B11111, B11011, B11101, B00000, B11101, B11011, B11111,
};

byte setButtonIcon[8] = {
    B11111, B11011, B11011, B10001, B11011, B11011, B11111,
};

byte pumpIcon[8] = {
    B00000, B00100, B00100, B01110, B11111, B11111, B01110,
};

/*****************/
/****** ISR ******/
/*****************/

void ISR_ButtonMode() { buttonModeWasPressed = true; }

void ISR_ButtonSet() { buttonSetWasPressed = true; }

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
  pinMode(LEVEL_PIN, INPUT);
  pinMode(LCD_A, OUTPUT);

  pinMode(BUTTON_MODE_PIN, INPUT);
  pinMode(BUTTON_SET_PIN, INPUT);

  pinMode(PUMP0_PIN, OUTPUT);
  pinMode(PUMP1_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(BUTTON_MODE_PIN), ISR_ButtonMode, RISING);
  attachInterrupt(digitalPinToInterrupt(BUTTON_SET_PIN), ISR_ButtonSet, RISING);
}

void setupLcd() {
  lcd.begin(16, 2);
  lcd.createChar(1, modeButtonIcon);
  lcd.createChar(2, setButtonIcon);
  lcd.createChar(3, pumpIcon);
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
#else
  WDTCSR = 1 << WDP0 | 1 << WDP3; // Set new watchdog timeout prescaler value (8.192 seconds)
#endif // CYCLE_OF_8S

#endif                 // FAST
  WDTCSR |= _BV(WDIE); // Enable the WD interrupt (note no reset)
}

void setup() {
  setupPins();
  setupLcd();
  setupLog();
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
/*****  LEVEL  *****/
/*****************/

int readLevel() {
  return digitalRead(LEVEL_PIN);
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

void pumpControl(int aState, int pin) {
  if (aState && bot.getState() == RunState) {
    digitalWrite(pin, HIGH);
  } else {
    digitalWrite(pin, LOW);
  }
}

void lcdControl() {
  if (bot.getState() == RunState) {
    digitalWrite(LCD_A, LOW);
  } else {
    digitalWrite(LCD_A, HIGH);
  }
}

void loop() {

  log(Info, "\n\n\nLOOP");

  bot.cycle(buttonModeWasPressed, buttonSetWasPressed, wdtWasTriggered);

  if (wdtWasTriggered) {
    wdtWasTriggered = false;
  }

  lcdControl();
  pumpControl(pump0.getActorState(), PUMP0_PIN);
  pumpControl(pump1.getActorState(), PUMP1_PIN);

  if (buttonModeWasPressed || buttonSetWasPressed) {
    delay(BUTTON_DEBOUNCING_DELAY_MS);
    buttonModeWasPressed = false;
    buttonSetWasPressed = false;
  }

  enterSleep();
}

#endif
