#ifndef UNIT_TEST

#include <Main.h>

#define SERIAL_BAUDS 115200
#define BUTTON_DEBOUNCING_DELAY_MS 120

volatile bool wdtWasTriggered = true;       // flag related to periodic WDT interrupts
volatile bool buttonModeWasPressed = false; // flag related to mode button pressed
volatile bool buttonSetWasPressed = false;  // flag related to set button pressed
volatile bool acceptButtons = true;         // ignore buttons in some circumstances

Pump pump0(MSG_PUMP_NAME0);
Pump pump1(MSG_PUMP_NAME1);
Actor *pumps[] = {&pump0, &pump1};

Bot bot(displayOnLcdString, pumps, 2);
LiquidCrystal lcd(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

byte modeButtonIcon[8] = {
  B11111,
  B11011,
  B11101,
  B00000,
  B11101,
  B11011,
  B11111,
};

byte setButtonIcon[8] = {
  B11111,
  B11011,
  B11011,
  B10001,
  B11011,
  B11011,
  B11111,
};

byte pumpIcon[8] = {
  B00000,
  B00100,
  B00100,
  B01110,
  B11111,
  B11111,
  B01110,
};

/*****************/
/****** ISR ******/
/*****************/

void ISR_Button0() { buttonModeWasPressed = acceptButtons; }

void ISR_Button1() { buttonSetWasPressed = acceptButtons; }

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
  pinMode(LCD_A, OUTPUT);

  pinMode(BUTTON0, INPUT);
  pinMode(BUTTON1, INPUT);

  pinMode(PUMP0_PIN, OUTPUT);
  pinMode(PUMP1_PIN, OUTPUT);

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
  lcd.begin(16, 2);
  lcd.createChar(1, modeButtonIcon);
  lcd.createChar(2, setButtonIcon);
  lcd.createChar(3, pumpIcon);
  Serial.begin(SERIAL_BAUDS);
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

void highElectricalLoad() {
  acceptButtons = false; // noise can cause button interrupts
}

void lowElectricalLoad() {
  acceptButtons = true; // end of noise
}

void pumpControl() {
  if (pump0.getActorState() && bot.getState() == RunState) {
    highElectricalLoad();
    digitalWrite(PUMP0_PIN, HIGH);
  } else {
    digitalWrite(PUMP0_PIN, LOW);
    lowElectricalLoad();
  }
  if (pump1.getActorState() && bot.getState() == RunState) {
    highElectricalLoad();
    digitalWrite(PUMP1_PIN, HIGH);
  } else {
    digitalWrite(PUMP1_PIN, LOW);
    lowElectricalLoad();
  }
}

void loop() {

  stroboscope();

  log(Info, "\n\n\nLOOP");

  bot.cycle(buttonModeWasPressed, buttonSetWasPressed, wdtWasTriggered);

  if (bot.getState() == RunState) {
    digitalWrite(LCD_A, LOW);
    bot.nextInfoState();
  } else {
    digitalWrite(LCD_A, HIGH);
  }

  pumpControl();

  if (buttonModeWasPressed || buttonSetWasPressed) {
    delay(BUTTON_DEBOUNCING_DELAY_MS);
    buttonModeWasPressed = false;
    buttonSetWasPressed = false;
  }
  if (wdtWasTriggered) {
    wdtWasTriggered = false;
  }

  enterSleep();
}

#endif
