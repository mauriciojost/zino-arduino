#ifndef UNIT_TEST

const char* display(const char* msg) {
  Serial.println(msg);
  delay(DELAY_DEBUG_MS);
  return msg;
}

#else

// When UNIT_TEST
const char* display(const char* msg) {
  return msg;
}

#endif
