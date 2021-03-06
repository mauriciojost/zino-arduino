#ifdef SIMULATION_MODULE

// Auxiliary libraries
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>

// Library being tested
#include <Module.h>

Module m;
bool keep = true;

void clearScreen() {
  system("clear");
}

void sanitize(char *str) {
  for (int i; i< strlen(str); i++) {
    char c = str[i];
    switch (c) {
      case '\01':
      case '\02':
      case '\03':
        str[i] = '*';
        break;
    }
  }
}

void displayLcdMockupFunctionString(const char *str1, const char *str2) {

  char lcdContentUp[LCD_LENGTH + 1];
  char lcdContentDown[LCD_LENGTH + 1];

  strcpy(lcdContentUp, str1);
  strcpy(lcdContentDown, str2);

  sanitize(lcdContentUp);
  sanitize(lcdContentDown);

  printf("\n### LCD UPDATE:\n");
  printf("%s----------------%s\n", KWHTBLU, KNRM);
  printf("%s%s%s\n", KBLU, lcdContentUp, KNRM);
  printf("%s%s%s\n", KBLU, lcdContentDown, KNRM);
  printf("%s----------------%s\n\n\n", KWHTBLU, KNRM);
}

int readLevel() {
  int value = 0;
  printf("### LEVEL READ: %d\n", value);
  return value;
}

void digitalWriteMocked(unsigned char pin, unsigned char value) {
  printf("### PIN WRITE %d : %s\n", (int)pin, (value == 0? "LOW" : "HIGH"));
}

void showHelp() {
  printf("HELP\n");
  printf("m - mode button\n");
  printf("s - set button\n");
  printf("t - timing interrupt\n");
  printf("q - quit\n");
}

void moduleSetup(Module* m) {
  m->setup();
  m->setFactor(SECS_CYCLE_FACTOR_DEFAULT);
  m->setStdoutWriteFunction(displayLcdMockupFunctionString);
  m->setReadLevelFunction(readLevel);
  m->setDigitalWriteFunction(digitalWriteMocked);
}

void* timePasses(void*) {
  while(keep){
    clearScreen();
    m.loop(false, false, true);
    usleep(500 * 1000);
  }
}

int main() {

  moduleSetup(&m);

  pthread_t t1;

  pthread_create(&t1, NULL, &timePasses, NULL);

  char c;
  showHelp();
  do {
    c = getchar();
    switch (c) {
      case 'm':
      case 'M':
        clearScreen();
        m.loop(true, false, false); break;
      case 's':
      case 'S':
        clearScreen();
        m.loop(false, true, false); break;
      case 't':
      case 'T':
        clearScreen();
        m.loop(false, false, true); break;
      case 'q':
      case 'Q':
        keep = false; break;
      case 'h':
      case 'H':
        showHelp(); break;
      default:
        break;
    }
  } while(keep);

  void* result;
  pthread_join(t1,&result);

}

#endif // SIMULATION_MODULE
