#ifndef UNIT_TEST

#include <Main.h>
#ifdef DEBUG
#define BUTTON_DEBOUNCING_DELAY_MS 80
#else
#define BUTTON_DEBOUNCING_DELAY_MS 320
#endif // DEBUG
#define PUMP_ACTIVATION_OFFSET_UNIT 60

volatile bool wdtWasTriggered = true;       // flag related to periodic WDT interrupts
volatile bool buttonModeWasPressed = false; // flag related to mode button pressed
volatile bool buttonSetWasPressed = false;  // flag related to set button pressed

volatile bool overruns = 0; // counter to keep track of amount of timing
                            // interrupts lost because of overrun

const int amountOfActors = 3;
Pump p0(MSG_PUMP_NAME0);
Delayer pump0(&p0, PUMP_ACTIVATION_OFFSET_UNIT * 0);
Pump p1(MSG_PUMP_NAME1);
Delayer pump1(&p1, PUMP_ACTIVATION_OFFSET_UNIT * 1);

Level level(MSG_LEVEL_NAME, readLevel);
Actor *actors[amountOfActors] = {&pump0, &pump1, &level};

Bot bot(displayOnLcdString, actors, amountOfActors);
Lcd lcd(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

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
  pinMode(PUMP1_PIN, OUTPUT);

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
#else
  WDTCSR = 1 << WDP0 | 1 << WDP3; // Set new watchdog timeout prescaler value (8.192 seconds)
#endif // CYCLE_OF_8S

#endif                 // FAST
  WDTCSR |= _BV(WDIE); // Enable the WD interrupt (note no reset)
}

void setup() {
  setupPins();
  lcd.initialize();
  setupLog();
  setupWDT();
}

/*****************/
/*****  LCD  *****/
/*****************/

void displayOnLcdString(const char *str1, const char *str2) {
  lcd.display(str1, str2);
}

/*****************/
/***** LEVEL *****/
/*****************/

int readLevel() {
  log(Debug, "RDLVL");
  return digitalRead(LEVEL_ADC_PIN);
}

/*****************/
/***** MAIN  *****/
/*****************/

void enterSleep(void) {
  log(Info, "SLEEP");
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  digitalWrite(BUILTIN_LED, LOW);
  sleep_enable();
  sleep_mode();
  /*** THE PROGRAM WILL CONTINUE FROM HERE AFTER THE WDT TIMEOUT ***/
  digitalWrite(BUILTIN_LED, HIGH);
  sleep_disable();    // First thing to do is disable sleep
  power_all_enable(); // Re-enable the peripherals
}

void controlActuator(int aState, int pin) {
  if (aState && bot.getState() == RunState) {
    digitalWrite(pin, HIGH);
  } else {
    digitalWrite(pin, LOW);
  }
}

void loop() {

  bool localWdt = wdtWasTriggered;

  if (wdtWasTriggered) {
    wdtWasTriggered = false;
  }

  log(Info, "\n\n\nLOOP");

  // execute a cycle on the bot
  bot.cycle(buttonModeWasPressed && digitalRead(BUTTON_MODE_PIN),
            buttonSetWasPressed && digitalRead(BUTTON_SET_PIN),
            localWdt);


  bool onceIn5Cycles = (bot.getClock()->getSeconds() % 5) == 0;
  log(Debug, "OVRN: ", overruns);
  log(Debug, "1/5: ", onceIn5Cycles);

  digitalWrite(LCD_A, bot.getState() != RunState);
  controlActuator(pump0.getActuatorValue(), PUMP0_PIN);
  controlActuator(pump1.getActuatorValue(), PUMP1_PIN);
  controlActuator(level.getActuatorValue() && onceIn5Cycles, LEVEL_BUZZER_PIN);

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
