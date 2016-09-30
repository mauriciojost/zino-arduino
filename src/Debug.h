#ifndef UNIT_TEST

#ifndef DEBUG
#define DEBUG

#define DELAY_DEBUG_MS 100
size_t debug(const char *msg) {
  Serial.println(msg);
  delay(DELAY_DEBUG_MS);
}

size_t debug(int msg) {
  Serial.println(msg);
  delay(DELAY_DEBUG_MS);
}

#endif

#endif
