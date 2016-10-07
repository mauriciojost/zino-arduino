#define DELAY_DEBUG_MS 10

#ifndef DEBUG_INC
#define DEBUG_INC

#ifndef UNIT_TEST

  #include <Arduino.h>

  // !UNIT_TEST, SO ON-BOARD EXECUTION
  #ifdef DEBUG

    // Receive logs via serial port

    void debug(const char *msg) {
      Serial.println(msg);
      delay(DELAY_DEBUG_MS);
    }

    void debug(int msg) {
      Serial.println(msg);
      delay(DELAY_DEBUG_MS);
    }

  #else // !DEBUG

    // Do not generate logs
    void debug(const char *msg) {}
    void debug(int msg) {}

  #endif // DEBUG

#else // UNIT_TEST, SO ON-PC EXECUTION

  void debug(const char *msg) {
    printf("DEBUG: %s\n", msg);
  }
  void debug(int msg) {
    printf("DEBUG: %d\n", msg);
  }

  void delay(int ms) { }


#endif // UNIT_TEST

#endif // DEBUG_INC
