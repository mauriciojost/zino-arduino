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



#define DELAY 3000
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


LiquidCrystal lcd(LCD_RS_PIN, LCD_ENABLE_PIN, LCD_D4_PIN, LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);
Servo servo;
UI ui;

volatile int f_wdt=1;

/*
void timerIsr() {
  lcd.setCursor(0, 0); lcd.print("TIMIS"); delay(DELAY);
  ui.go(digitalRead(BUTTON0), digitalRead(BUTTON1), HIGH);
  digitalWrite(BUILTIN_LED, !digitalRead(BUILTIN_LED));
}

*/
void wakeUpButton0() {
  Serial.println("Int button 0");
}

void wakeUpButton1() {
  Serial.println("Int button 1");
}

/*
void sleepNow()         // here we put the arduino to sleep
{

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);   // sleep mode is set here

    sleep_enable();          // enables the sleep bit in the mcucr register
                             // so sleep is possible. just a safety pin

    attachInterrupt(digitalPinToInterrupt(BUTTON0), wakeUp, RISING);
    attachInterrupt(digitalPinToInterrupt(BUTTON1), wakeUp, RISING);

    lcd.setCursor(0, 0); lcd.print("SLEEP"); delay(DELAY);
    sleep_mode();            // here the device is actually put to sleep!!
                             // THE PROGRAM CONTINUES FROM HERE AFTER WAKING UP

    sleep_disable();         // first thing after waking from sleep:
                             // disable sleep...
                             // disables interrupt 0 on pin 2 so the
                             // wakeUpNow code will not be executed
                             // during normal running time.
    lcd.setCursor(0, 0); lcd.print("AWAKE"); delay(DELAY);
    detachInterrupt(digitalPinToInterrupt(BUTTON0));
    detachInterrupt(digitalPinToInterrupt(BUTTON1));


}
*/
/*
void setup() {
  //Timer1.initialize(100000); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
  Timer1.attachInterrupt(timerIsr); // attach the service routine here

}


void loop() {
  lcd.setCursor(0, 0); lcd.print("LOOPI"); delay(DELAY);
  Timer1.initialize(ui.get_timer_period_ms());
  sleepNow();
  Timer1.initialize(ui.get_timer_period_ms());
}
*/


ISR(WDT_vect) {
  if(f_wdt == 0) {
    f_wdt=1;
  } else {
    Serial.println("WDT Overrun!!!");
  }
}


/***************************************************
 *  Name:        enterSleep
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Enters the arduino into sleep mode.
 *
 ***************************************************/
void enterSleep(void)
{
  set_sleep_mode(SLEEP_MODE_PWR_SAVE);   /* EDIT: could also use SLEEP_MODE_PWR_DOWN for lowest power consumption. */
  sleep_enable();

  /* Now enter sleep mode. */
  sleep_mode();

  /* The program will continue from here after the WDT timeout*/
  sleep_disable(); /* First thing to do is disable sleep. */

  /* Re-enable the peripherals. */
  power_all_enable();
}

void setupWDT() {
    MCUSR &= ~(1<<WDRF); // Clear the reset flag.
    WDTCSR |= (1<<WDCE) | (1<<WDE); // In order to change WDE or the prescaler, we need to set WDCE (This will allow updates for 4 clock cycles).
    WDTCSR = 1<<WDP0 | 1<<WDP3; // Set new watchdog timeout prescaler value (8.0 seconds).
    WDTCSR |= _BV(WDIE); // Enable the WD interrupt (note no reset).
}

void setup() {

  Serial.begin(9600);
  Serial.println("Initialising...");

  pinMode(BUTTON0, INPUT);
  pinMode(BUTTON1, INPUT);
  pinMode(BUILTIN_LED, OUTPUT);

  servo.attach(SERVO);

  delay(100); //Allow for serial print to complete.

  attachInterrupt(digitalPinToInterrupt(BUTTON0), wakeUpButton0, RISING);
  attachInterrupt(digitalPinToInterrupt(BUTTON1), wakeUpButton1, RISING);

  setupWDT();

  Serial.println("Initialisation complete.");
  delay(100); //Allow for serial print to complete.

}



/***************************************************
 *  Name:        enterSleep
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Main application loop.
 *
 ***************************************************/
void loop()
{
  if(f_wdt == 1)
  {
    /* Toggle the LED */
    digitalWrite(BUILTIN_LED, !digitalRead(BUILTIN_LED));
    Serial.println("Timer interrupt");
    delay(100); //Allow for serial print to complete.

    /* Don't forget to clear the flag. */
    f_wdt = 0;

    /* Re-enter sleep mode. */
    enterSleep();
  }
  else
  {
    /* Do nothing. */
  }
}
