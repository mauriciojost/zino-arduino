#ifndef UNIT_TEST

char* display(char* msg) {
  Serial.println(msg);
  delay(DELAY_DEBUG_MS);
  return msg;
}

#else

// When UNIT_TEST
char* display(char* msg) {
  return msg;
}

#endif
