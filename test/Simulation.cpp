#ifdef SIMULATION_MODULE

// Auxiliary libraries
#include <stdio.h>
#include <string.h>
#include <pthread.h>

// Library being tested
#include <Module.h>

const char *EMPTY_MSG = "";

const char **lcdContentUp = &EMPTY_MSG;
const char **lcdContentDown = &EMPTY_MSG;

void setUp(void) {
  lcdContentUp = &EMPTY_MSG;
  lcdContentDown = &EMPTY_MSG;
}

void tearDown(void) {}

void displayLcdMockupFunctionString(const char *str1, const char *str2) {

  lcdContentUp = &str1;
  lcdContentDown = &str2;

  printf("\n### LCD UPDATE:\n");
  printf("%s----------------%s\n", KWHTBLU, KNRM);
  printf("%s%s%s\n", KBLU, *lcdContentUp, KNRM);
  printf("%s%s%s\n", KBLU, *lcdContentDown, KNRM);
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
int main() {

  Module m;
  moduleSetup(&m);

  bool keep = true;
  char c;
  showHelp();
  do {
    c = getchar();
    switch (c) {
      case 'm':
      case 'M':
        m.loop(true, false, false); break;
      case 's':
      case 'S':
        m.loop(false, true, false); break;
      case 't':
      case 'T':
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
}

#endif // SIMULATION_MODULE
