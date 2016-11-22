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
/*****  MISC  *****/
/******************/

void displayOnLcdString(const char *str1, const char *str2) {
  m.lcd->display(str1, str2);
}

void saveFactor(bool setPressed) {
  if (setPressed) {
    float eepromFactor;
    EEPROM.get(FACTOR_EEPROM_ADDRESS, eepromFactor);
    float clockFactor = m.bot->getClock()->getFactor();
    if (clockFactor != eepromFactor) {
      log(CLASS, Debug, "F (PUT) : ", (int)(clockFactor*10000));
      EEPROM.put(FACTOR_EEPROM_ADDRESS, clockFactor);
    }
  }
}

int readLevel() {
  log(CLASS, Debug, "RDLVL");
  pinMode(LEVEL_VCC_PIN, OUTPUT);
  digitalWrite(LEVEL_VCC_PIN, HIGH);
  delay(LEVEL_VCC_MEASURE_DELAY_MS);
  int level = digitalRead(LEVEL_ADC_PIN);
  digitalWrite(LEVEL_VCC_PIN, LOW);
  pinMode(LEVEL_VCC_PIN, INPUT);
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
  m.lcd->initialize();
  setupLog();
  setupWDT();
  m.clock->setFactor(setupFactor());
  m.bot->setStdoutFunction(displayOnLcdString);
  m.level->setReadLevelFunction(readLevel);
}

/*****************/
/***** MAIN  *****/
/*****************/

void enterSleep(void) {
  log(CLASS, Info, "SLEEP");
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // SLEEP_MODE_IDLE
  digitalWrite(BUILTIN_LED, LOW);
  sleep_enable();
  sleep_mode();
  /*** THE PROGRAM WILL CONTINUE FROM HERE AFTER THE WDT TIMEOUT ***/
  digitalWrite(BUILTIN_LED, HIGH);
  sleep_disable();    // First thing to do is disable sleep
  power_all_enable(); // Re-enable the peripherals
}

void controlActuator(int aState, int pin) {
  if (aState && m.bot->getMode() == RunMode) {
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

  log(CLASS, Info, "\n\n\nLOOP");

  // execute a cycle on the bot
  m.bot->cycle(buttonModeWasPressed && digitalRead(BUTTON_MODE_PIN),
               buttonSetWasPressed && digitalRead(BUTTON_SET_PIN),
               localWdt);


  bool onceIn5Cycles = (m.bot->getClock()->getSeconds() % 5) == 0;
  log(CLASS, Debug, "OVRN: ", overruns);
  log(CLASS, Debug, "1/5: ", onceIn5Cycles);

  digitalWrite(LCD_A, m.bot->getMode() != RunMode);
  controlActuator(m.pump0->getActuatorValue(), PUMP0_PIN);
  controlActuator(m.pump1->getActuatorValue(), PUMP1_PIN);
  controlActuator(m.level->getActuatorValue() && onceIn5Cycles, LEVEL_BUZZER_PIN);

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
