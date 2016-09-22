/*
 * The LCD circuit:
 *  LCD RS pin to digital pin 12
 *  LCD Enable pin to digital pin 11
 *  LCD D4 pin to digital pin 5
 *  LCD D5 pin to digital pin 4
 *  LCD D6 pin to digital pin 3
 *  LCD D7 pin to digital pin 2
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
 */

// include the library code:
#include <LiquidCrystal.h>
#include <Arduino.h>
#include <Servo.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

const int INCREMENT = 1;
const int MIN_ANGLE = -20;
const int MAX_ANGLE = 200;

Servo servo1;
Servo servo2;

bool selector = false;
int pos1 = 0;
int pos2 = 0;


void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("ZORRINO");

  pinMode(A0, INPUT);
  pinMode(A1, INPUT);

  pinMode(13, OUTPUT);
  digitalWrite(13, selector);

  servo1.attach(A2);
  servo2.attach(A3);
}

void loop() {
  int a0 = digitalRead(A0);
  int a1 = digitalRead(A1);

  if (a0 == HIGH && a1 == HIGH) {
    selector = !selector;
    digitalWrite(13, selector);
    delay(100);
  } else if (a0 == HIGH) {
    if (selector) {
      pos1 = constrain(pos1 + INCREMENT, MIN_ANGLE, MAX_ANGLE);
    } else {
      pos2 = constrain(pos2 + INCREMENT, MIN_ANGLE, MAX_ANGLE);
    }
  } else if (a1 == HIGH) {
    if (selector) {
      pos1 = constrain(pos1 - INCREMENT, MIN_ANGLE, MAX_ANGLE);
    } else {
      pos2 = constrain(pos2 - INCREMENT, MIN_ANGLE, MAX_ANGLE);
    }
  }
  if (selector) {
    servo1.write(pos1);
  } else {
    servo2.write(pos2);
  }

  delay(100);
}
