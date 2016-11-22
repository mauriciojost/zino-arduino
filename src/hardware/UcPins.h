#ifndef UC_PINS_INC
#define UC_PINS_INC

#define UC_PIN_1 1
#define UC_PIN_2 2
#define UC_PIN_3 3
#define UC_PIN_4 4
#define UC_PIN_5 5
#define UC_PIN_6 6
#define UC_PIN_7 7
#define UC_PIN_8 8
#define UC_PIN_9 9
#define UC_PIN_10 10
#define UC_PIN_11 11
#define UC_PIN_12 12
#define UC_PIN_13 13

#ifndef UNIT_TEST // On arduino

#define UC_PIN_A0 A0
#define UC_PIN_A1 A1
#define UC_PIN_A2 A2
#define UC_PIN_A3 A3
#define UC_PIN_A4 A4
#define UC_PIN_A5 A5
#define UC_PIN_A6 A6
#define UC_PIN_A7 A7

#else // UNIT_TEST, on PC

#define UC_PIN_A0 0
#define UC_PIN_A1 1
#define UC_PIN_A2 2
#define UC_PIN_A3 3
#define UC_PIN_A4 4
#define UC_PIN_A5 5
#define UC_PIN_A6 6
#define UC_PIN_A7 7

#define LOW 0
#define HIGH 1

#endif // UNIT_TEST

#endif // UC_PINS_INC
