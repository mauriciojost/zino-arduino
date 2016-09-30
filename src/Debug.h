#ifndef UNIT_TEST
size_t debug(const char *msg) {
  Serial.println(msg);
  delay(100);
}

size_t debug(int msg) {
  Serial.println(msg);
  delay(100);
}
#endif
