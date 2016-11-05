#ifndef APP_PINS_INC
#define APP_PINS_INC

// ONBOARD
#define BUILTIN_LED UC_PIN_13

// BUTTONS
#define BUTTON0 UC_PIN_2
#define BUTTON1 UC_PIN_3

// PUMP
#define PUMP0_PIN UC_PIN_10
#define PUMP1_PIN UC_PIN_9

// LEVEL
#define LEVEL_PIN UC_PIN_8

// LCD
#define LCD_VSS GROUND
#define LCD_VCC VCC
#define LCD_V0 GROUND
#define LCD_RS_PIN UC_PIN_12
#define LCD_RW GROUND
#define LCD_ENABLE_PIN UC_PIN_11
#define LCD_D0_PIN AIR
#define LCD_D1_PIN AIR
#define LCD_D2_PIN AIR
#define LCD_D3_PIN AIR
#define LCD_D4_PIN UC_PIN_4
#define LCD_D5_PIN UC_PIN_5
#define LCD_D6_PIN UC_PIN_6
#define LCD_D7_PIN UC_PIN_7
#define LCD_A UC_PIN_A0 // via RES of 1K
#define LCD_K GROUND

#endif // APP_PINS_INC
